add_subdirectory(complex)

add_executable(complex_test complex_test.cpp)
target_link_libraries(complex_test complex)