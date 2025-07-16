cmake_minimum_required(VERSION 3.10)
project(HashTable)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(test_hashtable test_hashtable.cpp)

target_include_directories(test_hashtable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME HashTableTests COMMAND test_hashtable) 
