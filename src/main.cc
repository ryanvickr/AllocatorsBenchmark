#include <benchmark/benchmark.h>

#include <iostream>
#include <list>

#include "allocators/custom-allocator.h"

// This is great for when we have some sort of small local blob/data that needs
// to be sent onwards for processing (e.g. serialization/deserialization). We
// use a heap-allocated buffer here.
static void BM_VectorHeapAllocated(benchmark::State& state) {
  std::unique_ptr<char[]> buffer(new char[1024]);  // heap allocated buffer
  for (const auto& _ : state) {
    std::pmr::monotonic_buffer_resource bytes(buffer.get(), sizeof(buffer));
    std::pmr::vector<uint8_t> vec(&bytes);
    for (int i = 0; i < 50; i++) {
      vec.push_back(i);
    }
  }
}

// This is great for when we have some sort of small local blob/data that needs
// to be sent onwards for processing (e.g. serialization/deserialization). We
// can use a stack-allocated buffer here.
static void BM_VectorStackAllocated(benchmark::State& state) {
  char buffer[1024];  // stack allocated buffer
  for (const auto& _ : state) {
    std::pmr::monotonic_buffer_resource bytes(buffer, sizeof(buffer));
    std::pmr::vector<uint8_t> vec(&bytes);
    for (int i = 0; i < 50; i++) {
      vec.push_back(i);
    }
  }
}

static void BM_VectorVanilla(benchmark::State& state) {
  for (const auto& _ : state) {
    std::vector<uint8_t> vec;
    for (int i = 0; i < 50; i++) {
      vec.push_back(i);
    }
  }
}
BENCHMARK(BM_VectorVanilla)->Threads(1);
BENCHMARK(BM_VectorStackAllocated)->Threads(1);
BENCHMARK(BM_VectorHeapAllocated)->Threads(1);
BENCHMARK(BM_VectorVanilla)->Threads(5);
BENCHMARK(BM_VectorStackAllocated)->Threads(5);
BENCHMARK(BM_VectorHeapAllocated)->Threads(5);
BENCHMARK(BM_VectorVanilla)->Threads(10);
BENCHMARK(BM_VectorStackAllocated)->Threads(10);
BENCHMARK(BM_VectorHeapAllocated)->Threads(10);

template <typename Allocator>
static void BM_CreateAndAccess(benchmark::State& state) {
  static_assert(std::is_base_of<allocators::CustomAllocator, Allocator>(),
                "Allocator must be derived from allocators::CustomAllocator");

  // The worker lambda
  auto worker = []() {
    Allocator alloc;
    std::pmr::list<int16_t> values(alloc.get_resource());

    for (int i = 0; i < 100000; i++) {
      values.push_back(i);
    }

    for (int i = 0; i < 1000; i++) {
      for (auto& value : values) {
        ++value;
      }
      values.erase(values.begin());
      values.push_back(0);
    }

    return values.back();
  };

  for (const auto& _ : state) {
    const auto value = worker();
  }
}
// Single threaded
BENCHMARK(BM_CreateAndAccess<allocators::StandardAllocator>)->Threads(1);
BENCHMARK(BM_CreateAndAccess<allocators::MonotonicAllocator<10>>)->Threads(1);
BENCHMARK(BM_CreateAndAccess<allocators::PoolMonotonicAllocator<10>>)
    ->Threads(1);
// 5 threads
BENCHMARK(BM_CreateAndAccess<allocators::StandardAllocator>)->Threads(5);
BENCHMARK(BM_CreateAndAccess<allocators::MonotonicAllocator<10>>)->Threads(5);
BENCHMARK(BM_CreateAndAccess<allocators::PoolMonotonicAllocator<10>>)
    ->Threads(5);
// 10 threads
BENCHMARK(BM_CreateAndAccess<allocators::StandardAllocator>)->Threads(10);
BENCHMARK(BM_CreateAndAccess<allocators::MonotonicAllocator<10>>)->Threads(10);
BENCHMARK(BM_CreateAndAccess<allocators::PoolMonotonicAllocator<10>>)
    ->Threads(10);

BENCHMARK_MAIN();
