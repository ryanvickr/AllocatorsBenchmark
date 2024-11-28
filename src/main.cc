#include <benchmark/benchmark.h>

#include <iostream>
#include <list>

#include "allocators/custom-allocator.h"

// This is great for when we have some sort of small local blob/data that needs
// to be sent onwards for processing (e.g. serialization/deserialization).
template <typename Allocator>
static void BM_Vector(benchmark::State& state) {
  static_assert(std::is_base_of<allocators::CustomAllocator, Allocator>(),
                "Allocator must be derived from allocators::CustomAllocator");

  Allocator alloc;
  for (const auto& _ : state) {
    std::pmr::vector<uint8_t> vec(alloc.get_resource());
    for (int i = 0; i < 100; i++) {
      vec.push_back(i);
    }
  }
}

BENCHMARK(BM_Vector<allocators::StandardAllocator>)->Threads(1);
BENCHMARK(BM_Vector<allocators::MonotonicAllocator<256>>)->Threads(1);
BENCHMARK(BM_Vector<allocators::PoolMonotonicAllocator<256>>)->Threads(1);
BENCHMARK(BM_Vector<allocators::StandardAllocator>)->Threads(5);
BENCHMARK(BM_Vector<allocators::MonotonicAllocator<256>>)->Threads(5);
BENCHMARK(BM_Vector<allocators::PoolMonotonicAllocator<256>>)->Threads(5);
BENCHMARK(BM_Vector<allocators::StandardAllocator>)->Threads(10);
BENCHMARK(BM_Vector<allocators::MonotonicAllocator<256>>)->Threads(10);
BENCHMARK(BM_Vector<allocators::PoolMonotonicAllocator<256>>)->Threads(10);

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
