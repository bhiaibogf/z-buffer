cmake_minimum_required(VERSION 3.16)
project(z_buffer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB MODEL ./model/*.cpp)

find_package(OpenCV REQUIRED)
add_executable(z_buffer main.cpp ${MODEL} camera.cpp rasterizer.cpp obj_loader.cpp)
target_link_libraries(z_buffer ${OpenCV_LIBRARIES})
