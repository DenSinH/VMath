cmake_minimum_required(VERSION 3.19)
project(VMath)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("-march=native -O3")

add_library(VMath Vector.h Types.h Matrix.h)
set_target_properties(VMath PROPERTIES LINKER_LANGUAGE CXX)

add_executable(VMathExample example/main.cpp)