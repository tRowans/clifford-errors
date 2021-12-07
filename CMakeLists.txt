cmake_minimum_required(VERSION 3.10)

project(czErrors)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(lib/blossom5)
add_subdirectory(lib/googletest)
add_subdirectory(lib/bp_osd)
add_subdirectory(src)
add_subdirectory(tst)
