cmake_minimum_required(VERSION 3.21)
project(SockDemo)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32)
add_executable(main.cpp)