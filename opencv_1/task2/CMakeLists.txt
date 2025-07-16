cmake_minimum_required(VERSION 3.10)
project(square_spiral)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(square_spiral spiral_generator.cpp)
target_link_libraries(square_spiral ${OpenCV_LIBS})