#ifndef ALLOCATORS_CUSTOM_ALLOCATOR_H
#define ALLOCATORS_CUSTOM_ALLOCATOR_H

#include <memory_resource>

namespace allocators {

// Pure virtual base class for custom allocators.
class CustomAllocator {
 public:
  virtual ~CustomAllocator() = default;

  // Returns the resource for this allocator.
  virtual std::pmr::memory_resource* get_resource() = 0;
};

// This is a typical allocator which uses new and delete.
class StandardAllocator : public CustomAllocator {
 public:
  StandardAllocator() = default;

  std::pmr::memory_resource* get_resource() override;
};

// This is a monotonic allocator which accepts a fixed size and allocates a
// sequential memory buffer.
template <std::size_t Size>
class MonotonicAllocator : public CustomAllocator {
 public:
  MonotonicAllocator() = default;

  std::pmr::memory_resource* get_resource() override {
    return &memory_resource_;
  }

 private:
  std::pmr::monotonic_buffer_resource memory_resource_{Size};
};

// This allocator. TODO: Finish
template <std::size_t Size>
class PoolMonotonicAllocator : public CustomAllocator {
 public:
  PoolMonotonicAllocator() = default;

  std::pmr::memory_resource* get_resource() override {
    return &memory_resource_;
  }

 private:
  std::pmr::monotonic_buffer_resource upstream_{Size};
  std::pmr::synchronized_pool_resource memory_resource_{&upstream_};
};

}  // namespace allocators

#endif  // ALLOCATORS_CUSTOM_ALLOCATOR_H
