cmake_minimum_required(VERSION 3.21)
project(SERVEdemo)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32)
add_executable(SERVEdemo main.cpp)
