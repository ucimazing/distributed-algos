#include <benchmark/benchmark.h>
#include <string>
#include "dsalgos/bloom_filter.hpp"
#include "dsalgos/murmurhash3.hpp"

using dsalgos::BloomFilter;
using dsalgos::MurmurHash64;

static void BM_BloomInsert(benchmark::State& state) {
    BloomFilter<std::string, MurmurHash64> bf(100000, 0.01);

    for (auto _ : state) {
        bf.add("user_" + std::to_string(state.iterations()));
    }
}

static void BM_BloomQueryHit(benchmark::State& state) {
    BloomFilter<std::string, MurmurHash64> bf(100000, 0.01);
    bf.add("umesh");

    for (auto _ : state) {
        benchmark::DoNotOptimize(bf.possibly_contains("umesh"));
    }
}

static void BM_BloomQueryMiss(benchmark::State& state) {
    BloomFilter<std::string, MurmurHash64> bf(100000, 0.01);

    for (auto _ : state) {
        benchmark::DoNotOptimize(bf.possibly_contains("not-present-key"));
    }
}

BENCHMARK(BM_BloomInsert);
BENCHMARK(BM_BloomQueryHit);
BENCHMARK(BM_BloomQueryMiss);

BENCHMARK_MAIN();