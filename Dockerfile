# syntax=docker/dockerfile:1

# ---- Stage 1: Builder ----
FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the entire project into the container
COPY . .

# Configure and build
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build --config Release

# ---- Stage 2: Runtime ----
FROM ubuntu:22.04

WORKDIR /app

# Copy compiled binaries from build stage
# Adjust paths if needed depending on where CMake outputs them
COPY --from=build /app/build/bloom_demo .
COPY --from=build /app/build/bloom_bench .
COPY --from=build /app/build/consistent_hash_bench .

# Default command: run the demo
CMD ["./bloom_demo"]