cmake_minimum_required(VERSION 3.10)
project(custom_kernel)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(lab2 lab2.cpp)
target_link_libraries(lab2 ${OpenCV_LIBS}) 