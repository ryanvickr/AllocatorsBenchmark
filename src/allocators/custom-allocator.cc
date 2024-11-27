#include "custom-allocator.h"

#include <memory_resource>

namespace allocators {

std::pmr::memory_resource* StandardAllocator::get_resource() {
  return std::pmr::new_delete_resource();
}

}  // namespace allocators
