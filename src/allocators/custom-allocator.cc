#include "custom-allocator.h"

namespace allocators {

std::pmr::memory_resource* StandardAllocator::get_resource() {
  return std::pmr::new_delete_resource();
}

// template <std::size_t Size>
// std::pmr::memory_resource* MonotonicAllocator<Size>::get_resource() {
//   return &memory_resource_;
// }

}  // namespace allocators
