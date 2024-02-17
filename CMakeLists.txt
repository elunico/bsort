cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_FLAGS "-O3 -std=c++20 -Wall -Wextra -fno-rtti -fno-exceptions")

project(bsort)

add_executable(bsort main.cpp)
