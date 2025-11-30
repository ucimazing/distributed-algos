cmake_minimum_required(VERSION 3.15)
project(distributed_algos)

set(CMAKE_CXX_STANDARD 17)

add_library(distributed_algos
    src/bloom_filter.cpp
    src/consistent_hash.cpp
)

target_include_directories(distributed_algos PUBLIC include)
