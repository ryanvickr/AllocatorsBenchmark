#ifndef ALLOCATORS_CUSTOM_ALLOCATOR_H
#define ALLOCATORS_CUSTOM_ALLOCATOR_H

#include <memory_resource>

namespace allocators {

// Pure virtual base class for custom allocators.
class CustomAllocator {
 public:
  virtual ~CustomAllocator() = default;

  virtual std::pmr::memory_resource* get_resource() = 0;
};

// This is a typical allocator which uses new and delete.
class StandardAllocator : public CustomAllocator {
 public:
  StandardAllocator() = default;

  std::pmr::memory_resource* get_resource() override;
};

}  // namespace allocators

#endif  // ALLOCATORS_CUSTOM_ALLOCATOR_H
