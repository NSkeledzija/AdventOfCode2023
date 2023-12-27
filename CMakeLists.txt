cmake_minimum_required(VERSION 3.10)

project(aoc2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)
target_include_directories(main PUBLIC "/home/nskeledz/projects/AdventOfCode2023")