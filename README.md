# Allocator Benchmarking

**Note: This is a work-in-progress!**

This is just a short little sandbox I created to test out various types of allocators. I used [google/benchmark](https://github.com/google/benchmark) to benchmark things.

## Methodology

Currently, I am testing with the following 2 allocators:

* Standard new and delete allocator
* Monotonic allocator
* Pool-Monotonic allocator

I have a simple "worker" function which loads and access elements in a container. This worker is tested 5 iterations each with 1 worker thread, 5 worker threads, and 10 worker threads.

## Findings

### std::list

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

Here I found that for the initial allocation of a small amount of data, things were pretty fast (~300% improvement). However, as the amount of data increased, the performance gain became almost negligble. The result below are shown with about 256 bytes of data:

| Allocator | # Threads | Runtime(ns) |
|---|:---:|---|
|Standard|1|292|
|Monotonic|1|89|
|Pool-Monotonic|1|161|
|Standard|5|321|
|Monotonic|5|100|
|Pool-Monotonic|5|177|
|Standard|10|421|
|Monotonic|10|137|
|Pool-Monotonic|10|229|
