#include <iostream>
#include <string>
#include <unordered_map>

#include "dsalgos/bloom_filter.hpp"
#include "dsalgos/consistent_hash.hpp"

int main() {
    using dsalgos::BloomFilter;
    using dsalgos::ConsistentHashRing;

    std::cout << "=== Bloom Filter Demo ===\n";

    // Bloom filter demo
    BloomFilter<std::string> bf(1000, 0.01);

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

    std::cout << "\n=== Consistent Hashing Demo ===\n";

    // Consistent hashing demo
    ConsistentHashRing<std::string> ring(100); // 100 virtual nodes per physical node

    ring.add_node("node-A");
    ring.add_node("node-B");
    ring.add_node("node-C");

    std::cout << "Physical nodes:        " << ring.node_count() << "\n";
    std::cout << "Total virtual nodes:   " << ring.ring_size() << "\n";
    std::cout << "Vnodes per node:       " << ring.virtual_node_count_per_node() << "\n\n";

    // Simulate assigning keys (e.g., user IDs) to nodes
    std::string keys[] = {"user-1", "user-2", "user-3", "user-4", "user-5"};

    for (const auto& key : keys) {
        const auto& node = ring.get_node_for_key(key);
        std::cout << "Key '" << key << "' -> " << node << "\n";
    }

    std::cout << "\nNow removing node-B...\n\n";
    ring.remove_node("node-B");

    std::cout << "Physical nodes:        " << ring.node_count() << "\n";
    std::cout << "Total virtual nodes:   " << ring.ring_size() << "\n\n";

    for (const auto& key : keys) {
        const auto& node = ring.get_node_for_key(key);
        std::cout << "Key '" << key << "' -> " << node << "\n";
    }

    return 0;
}