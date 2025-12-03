#pragma once

#include <cmath>
#include <cstdint>
#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>

#include "dsalgos/murmurhash3.hpp"

namespace dsalgos {

// Compute optimal bit array size (m) given expected items (n) and false positive probability (p)
inline size_t compute_m(size_t n, double p) {
    if (n == 0) {
        throw std::invalid_argument("n must be > 0");
    }
    if (p <= 0.0 || p >= 1.0) {
        throw std::invalid_argument("p must be in (0, 1)");
    }
    const double ln2 = std::log(2.0);
    return static_cast<size_t>(
        std::ceil(-(static_cast<double>(n) * std::log(p)) / (ln2 * ln2))
    );
}

// Compute optimal number of hash functions (k)
inline size_t compute_k(size_t m, size_t n) {
    const double ln2 = std::log(2.0);
    return static_cast<size_t>(
        std::round((static_cast<double>(m) / static_cast<double>(n)) * ln2)
    );
}

/**
 * Generic Bloom Filter.
 *
 * T      - key type
 * Hasher - functor type: size_t or uint64_t operator()(const T&)
 */
template <typename T, typename Hasher = std::hash<T>>
class BloomFilter {
public:
    BloomFilter(size_t expected_items,
                double false_positive_prob,
                Hasher hasher = Hasher())
        : m_bits(0),
          k_hashes(0),
          n_expected(expected_items),
          hasher_(std::move(hasher))
    {
        if (expected_items == 0) {
            throw std::invalid_argument("expected_items must be > 0");
        }
        if (false_positive_prob <= 0.0 || false_positive_prob >= 1.0) {
            throw std::invalid_argument("false_positive_prob must be in (0,1)");
        }

        m_bits   = compute_m(expected_items, false_positive_prob);
        k_hashes = compute_k(m_bits, expected_items);

        const size_t bytes = (m_bits + 7) / 8;
        bit_array_.assign(bytes, 0);
    }

    void add(const T& item) {
        auto [h1, h2] = base_hashes(item);
        for (size_t i = 0; i < k_hashes; ++i) {
            uint64_t combined = h1 + i * h2;
            const size_t idx = static_cast<size_t>(combined % m_bits);
            set_bit(idx);
        }
    }

    bool possibly_contains(const T& item) const {
        auto [h1, h2] = base_hashes(item);
        for (size_t i = 0; i < k_hashes; ++i) {
            uint64_t combined = h1 + i * h2;
            const size_t idx = static_cast<size_t>(combined % m_bits);
            if (!get_bit(idx)) {
                return false; // definitely not present
            }
        }
        return true; // maybe present
    }

    size_t bit_size() const { return m_bits; }
    size_t num_hashes() const { return k_hashes; }
    size_t expected_items() const { return n_expected; }

private:
    size_t m_bits;      // number of bits in bit array
    size_t k_hashes;    // number of hash functions
    size_t n_expected;  // planned capacity
    Hasher hasher_;
    std::vector<uint8_t> bit_array_; // storage

    void set_bit(size_t idx) {
        const size_t byte_idx = idx / 8;
        const uint8_t mask   = static_cast<uint8_t>(1u << (idx % 8));
        bit_array_[byte_idx] |= mask;
    }

    bool get_bit(size_t idx) const {
        const size_t byte_idx = idx / 8;
        const uint8_t mask   = static_cast<uint8_t>(1u << (idx % 8));
        return (bit_array_[byte_idx] & mask) != 0;
    }

    std::pair<uint64_t, uint64_t> base_hashes(const T& item) const {
        uint64_t h1 = static_cast<uint64_t>(hasher_(item));
        // derive a second hash from h1 (double hashing technique)
        uint64_t h2 = h1;
        h2 ^= (h2 >> 33);
        h2 *= 0xff51afd7ed558ccdULL;
        h2 ^= (h2 >> 33);
        if (h2 == 0) {
            h2 = 0x9e3779b97f4a7c15ULL; // avoid step size 0
        }
        return {h1, h2};
    }
};

} // namespace dsalgos