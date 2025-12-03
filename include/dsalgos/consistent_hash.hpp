#pragma once

#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <functional>
#include <sstream>
#include <type_traits>
#include <cstdint>

#include "dsalgos/murmurhash3.hpp"

namespace dsalgos {

/**
 * A simple consistent hashing ring with virtual nodes.
 *
 * Template parameters:
 *   NodeId - type used to identify a physical node (e.g. std::string, int, etc.)
 *   Hash   - hash functor that hashes std::string
 *
 * Design:
 *   - We store a sorted map<hash_value, NodeId> to represent the ring.
 *   - Each physical node is represented by multiple virtual nodes on the ring.
 *   - For a given key, we hash the key, find the first hash >= key_hash,
 *     and if none exists, wrap to begin() (ring behavior).
 */
template <typename NodeId, typename Hash = MurmurHash64>
class ConsistentHashRing {
public:
    explicit ConsistentHashRing(std::size_t virtual_nodes = 100, Hash hash = Hash())
        : vnodes_(virtual_nodes),
          hasher_(std::move(hash))
    {
        if (vnodes_ == 0) {
            throw std::invalid_argument("virtual_nodes must be > 0");
        }
    }

    // Add a physical node and its virtual nodes to the ring.
    void add_node(const NodeId& node) {
        for (std::size_t i = 0; i < vnodes_; ++i) {
            std::string vnode_key = node_to_string(node) + "#" + std::to_string(i);
            std::uint64_t h = static_cast<std::uint64_t>(hasher_(vnode_key));
            ring_.emplace(h, node);
        }
    }

    // Remove a physical node and all its virtual nodes.
    void remove_node(const NodeId& node) {
        if (ring_.empty()) return;

        std::string base = node_to_string(node);
        for (std::size_t i = 0; i < vnodes_; ++i) {
            std::string vnode_key = base + "#" + std::to_string(i);
            std::uint64_t h = static_cast<std::uint64_t>(hasher_(vnode_key));

            auto range = ring_.equal_range(h);
            for (auto it = range.first; it != range.second; ) {
                if (it->second == node) {
                    it = ring_.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    // Given a key, find the node responsible for it.
    const NodeId& get_node_for_key(const std::string& key) const {
        if (ring_.empty()) {
            throw std::runtime_error("No nodes in ring");
        }

        std::uint64_t h = static_cast<std::uint64_t>(hasher_(key));
        auto it = ring_.lower_bound(h);
        if (it == ring_.end()) {
            it = ring_.begin(); // wrap around
        }
        return it->second;
    }

    // Number of physical nodes currently present.
    std::size_t node_count() const {
        std::set<NodeId> unique_nodes;
        for (const auto& [hash, node] : ring_) {
            (void)hash;
            unique_nodes.insert(node);
        }
        return unique_nodes.size();
    }

    // Number of virtual nodes on the ring (i.e. map size).
    std::size_t ring_size() const {
        return ring_.size();
    }

    std::size_t virtual_node_count_per_node() const {
        return vnodes_;
    }

private:
    std::size_t vnodes_;
    Hash hasher_;
    // hash of virtual node -> physical NodeId
    std::map<std::uint64_t, NodeId> ring_;

    // Helper to convert NodeId to std::string
    static std::string node_to_string(const NodeId& node) {
        if constexpr (std::is_same_v<NodeId, std::string>) {
            return node;
        } else {
            std::ostringstream oss;
            oss << node; // requires operator<< for NodeId
            return oss.str();
        }
    }
};

} // namespace dsalgos