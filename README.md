# Allocator Benchmarking

**Note: This is a work-in-progress!**

This is just a short little sandbox I created to test out various types of allocators. I used [google/benchmark](https://github.com/google/benchmark) to benchmark things.

## Methodology

Currently, I am testing with the following 2 allocators:

* Standard new and delete allocator
* Monotonic allocator

I have a simple "worker" function which loads and access elements in a container. This worker is tested 5 iterations each with 1 worker thread, 5 worker threads, and 10 worker threads.

## Findings

### std::list

I started with a simple `std::list` to make things simple. Results:

| Allocator | # Threads | Runtime(ns) |
|---|:---:|---|
|Standard|1|138101808|
|Monotonic|1|122417153|
|Standard|5|180667683|
|Monotonic|5|135416150|
|Standard|10|249286617|
|Monotonic|10|180203150|

As we can see, the monotonic allocator scales with threads much better than the standard allocator. With 10 threads, we see ~30% performance improvement.

### std::vector

Here I found that for the initial allocation of a small amount of data, things were pretty fast (~300% improvement). However, as the amount of data increased, the performance gain became almost negligble. The result below are shown with about 256 bytes of data:

| Allocator | # Threads | Runtime(ns) |
|---|:---:|---|
|Standard|1|311|
|Monotonic|1|91|
|Standard|5|317|
|Monotonic|5|99|
|Standard|10|423|
|Monotonic|10|135|
