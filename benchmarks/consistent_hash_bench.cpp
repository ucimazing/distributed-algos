#include <benchmark/benchmark.h>
#include <string>
#include "dsalgos/consistent_hash.hpp"
#include "dsalgos/murmurhash3.hpp"

using dsalgos::ConsistentHashRing;
using dsalgos::MurmurHash64;

static void BM_ConsistentHashLookup(benchmark::State& state) {
    ConsistentHashRing<std::string, MurmurHash64> ring(200);
    ring.add_node("A");
    ring.add_node("B");
    ring.add_node("C");
    ring.add_node("D");

    for (auto _ : state) {
        std::string key = "user_" + std::to_string(state.iterations());
        benchmark::DoNotOptimize(ring.get_node_for_key(key));
    }
}

BENCHMARK(BM_ConsistentHashLookup);
BENCHMARK_MAIN();