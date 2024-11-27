#include <benchmark/benchmark.h>

#include <iostream>
#include <vector>

#include "allocators/custom-allocator.h"

template <typename Allocator>
static void BM_CreateAndAccess(benchmark::State& state) {
  for (const auto& _ : state) {
    std::vector<int> test;
  }
}

BENCHMARK(BM_CreateAndAccess<allocators::StandardAllocator>)->Threads(5);

BENCHMARK_MAIN();
