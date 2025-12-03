cmake_minimum_required(VERSION 3.16)

# -------------------------------
# Project Setup
# -------------------------------
project(distributed_systems_algos_cpp LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------
# FetchContent for Google Benchmark
# -------------------------------
include(FetchContent)

# Disable Google Benchmark tests (faster build)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        google_benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
)

FetchContent_MakeAvailable(google_benchmark)

# -------------------------------
# Header-only library (dsalgos)
# -------------------------------
add_library(dsalgos INTERFACE)

target_include_directories(dsalgos
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------------
# Demo Executable
# -------------------------------
add_executable(bloom_demo
        src/main.cpp
)

target_link_libraries(bloom_demo
        PRIVATE
        dsalgos
)

# -------------------------------
# Benchmarks
# -------------------------------

# Bloom Filter Benchmark
add_executable(bloom_bench
        benchmarks/bloom_bench.cpp
)

target_link_libraries(bloom_bench
        PRIVATE
        dsalgos
        benchmark::benchmark
)

# Consistent Hash Benchmark
add_executable(consistent_hash_bench
        benchmarks/consistent_hash_bench.cpp
)

target_link_libraries(consistent_hash_bench
        PRIVATE
        dsalgos
        benchmark::benchmark
)