# Allocator Benchmarking

**Note: This is a work-in-progress!**

This is just a short little sandbox I created to test out various types of allocators. I used [google/benchmark](https://github.com/google/benchmark) to benchmark things.

I have a simple "worker" function which loads and access elements in a container. This worker is tested 5 iterations each with 1 worker thread, 5 worker threads, and 10 worker threads.

## Findings

### std::list

#### std::list Methodology

Currently, I am testing with the following 2 allocators:

* Standard new and delete allocator
* Monotonic allocator
* Pool-Monotonic allocator

#### std::list Results

I started with a simple `std::list` to make things simple. Results:

| Allocator | # Threads | Runtime(ns) |
|---|:---:|---|
|Standard|1|137028300|
|Monotonic|1|122061229|
|Pool-Monotonic|1|133408508|
|Standard|5|175430400|
|Monotonic|5|135071067|
|Pool-Monotonic|5|148492025|
|Standard|10|263696213|
|Monotonic|10|175365779|
|Pool-Monotonic|10|211868550|

As we can see, the monotonic allocator scales with threads much better than the standard allocator. With 10 threads, we see ~30% performance improvement.

### std::vector

#### std::vector Methodology

Currently, I am testing with the following 2 allocators:

* Standard new and delete allocator
* Stack-allocated monotonic allocator
* Heap-allocated monotonic allocator

#### std::vector Results

Here I found that for the initial allocation of a small amount of data, things with the stack-based allocator were pretty fast (~300% improvement). Even the heap-based monotonic allocator showed improvements (20%). However, as the amount of data increased, the performance gain became almost negligble. The result below are shown with ~1000 bytes of data:

| Allocator | # Threads | Runtime(ns) |
|---|:---:|---|
|Standard|1|216|
|Heap-Allocated|1|196|
|Stack-Allocated|1|59.2|
|Standard|5|245|
|Heap-Allocated|5|204|
|Stack-Allocated|5|62.7|
|Standard|10|324|
|Heap-Allocated|10|262|
|Stack-Allocated|10|78.8|
