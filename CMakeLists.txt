cmake_minimum_required(VERSION 3.16)

# 1. Name of the project
project(distributed_algos LANGUAGES CXX)

# 2. Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Header-only library target for our algorithms
add_library(dsalgos INTERFACE
        src/main.cpp)

# 4. Tell CMake where to find headers when someone links this target
target_include_directories(dsalgos
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 5. Executable that uses the library
add_executable(bloom_demo
        src/main.cpp
        src/main.cpp
)

# 6. Link executable with the header-only library
target_link_libraries(bloom_demo
        PRIVATE
        dsalgos
)