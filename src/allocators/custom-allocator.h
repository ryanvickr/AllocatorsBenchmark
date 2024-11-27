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

template <std::size_t Size>
class MonotonicAllocator : public CustomAllocator {
 public:
  MonotonicAllocator() = default;

  std::pmr::memory_resource* get_resource() override {
    return &memory_resource_;
  }

 private:
  std::pmr::monotonic_buffer_resource memory_resource_{Size};
  std::pmr::polymorphic_allocator<> alloc{&memory_resource_};
};

}  // namespace allocators

#endif  // ALLOCATORS_CUSTOM_ALLOCATOR_H
