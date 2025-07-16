cmake_minimum_required(VERSION 3.10)
project(lab1)

# Используем pkg-config для поиска OpenCV
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv4)

include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})

add_executable(lab1 lab1.cpp)
target_link_libraries(lab1 ${OPENCV_LIBRARIES}) 