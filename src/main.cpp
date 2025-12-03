#include <iostream>
#include <string>

#include "dsalgos/murmurhash3.hpp"
#include "dsalgos/bloom_filter.hpp"
#include "dsalgos/consistent_hash.hpp"

int main() {
    using dsalgos::BloomFilter;
    using dsalgos::ConsistentHashRing;
    using dsalgos::MurmurHash64;

    std::cout << "=== Bloom Filter Demo (MurmurHash) ===\n";

    // Expect ~1000 items, target false positive probability of 1%
    BloomFilter<std::string, MurmurHash64> bf(100000, 0.01);

    bf.add("umesh");
    bf.add("backend");
    bf.add("distributed");
    bf.add("systems");

    std::cout << std::boolalpha;
    std::cout << "Contains 'umesh'?       " << bf.possibly_contains("umesh") << "\n";
    std::cout << "Contains 'backend'?     " << bf.possibly_contains("backend") << "\n";
    std::cout << "Contains 'random_key'?  " << bf.possibly_contains("random_key") << "\n";

    std::cout << "\nBloom filter details:\n";
    std::cout << "  bit size (m):      " << bf.bit_size() << "\n";
    std::cout << "  hash functions (k):" << bf.num_hashes() << "\n";
    std::cout << "  expected items (n):" << bf.expected_items() << "\n";

    std::cout << "\n=== Consistent Hashing Demo (MurmurHash) ===\n";

    // Consistent hashing demo
    ConsistentHashRing<std::string> ring(100); // 100 virtual nodes per physical node, MurmurHash64 by default

    ring.add_node("node-A");
    ring.add_node("node-B");
    ring.add_node("node-C");

    std::cout << "Physical nodes:        " << ring.node_count() << "\n";
    std::cout << "Total virtual nodes:   " << ring.ring_size() << "\n";
    std::cout << "Vnodes per node:       " << ring.virtual_node_count_per_node() << "\n\n";

    // Simulate assigning keys (e.g., user IDs) to nodes
    std::string keys[] = {"user-1", "user-2", "user-3", "user-4", "user-5"};

    std::cout << "Key -> Node mapping (before removal):\n";
    for (const auto& key : keys) {
        const auto& node = ring.get_node_for_key(key);
        std::cout << "  '" << key << "' -> " << node << "\n";
    }

    std::cout << "\nNow removing node-B...\n\n";
    ring.remove_node("node-B");

    std::cout << "Physical nodes:        " << ring.node_count() << "\n";
    std::cout << "Total virtual nodes:   " << ring.ring_size() << "\n\n";

    std::cout << "Key -> Node mapping (after removal):\n";
    for (const auto& key : keys) {
        const auto& node = ring.get_node_for_key(key);
        std::cout << "  '" << key << "' -> " << node << "\n";
    }

    return 0;
}