cmake_minimum_required(VERSION 3.16)
project(z_buffer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC src/*.cpp)

find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(z_buffer ${SRC})
target_link_libraries(z_buffer ${OpenCV_LIBRARIES})

#add_executable(test test/main.cpp)
