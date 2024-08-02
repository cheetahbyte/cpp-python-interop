cmake_minimum_required(VERSION 3.28)
project(python_cpp_interop)

set(CMAKE_CXX_STANDARD 17)

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Include directories
include_directories(${Python3_INCLUDE_DIRS})

add_executable(python_cpp_interop main.cpp)

# Link libraries
target_link_libraries(python_cpp_interop ${Python3_LIBRARIES})