#include <iostream>
#include <vector>

void standardAllocator() {
    std::vector<int, std::pmr::polymorphic_allocator<int>> test;
}

int main () {
    std::clog << "Hello world\n";
}