cmake_minimum_required(VERSION 3.5.0)
project(AllocatorsBenchmark VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

set ( SOURCES
  src/main.cc
)

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

add_executable(AllocatorsBenchmark ${SOURCES})
