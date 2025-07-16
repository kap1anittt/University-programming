cmake_minimum_required(VERSION 3.20)
project(stackarrt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

add_executable(test_stackarrt test_stackarrt.cpp)
target_include_directories(test_stackarrt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../dynamic_array)
