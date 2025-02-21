cmake_minimum_required(VERSION 3.29)

project(proj)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_executable(main start.cpp)

target_sources(main PRIVATE error_code.cpp Client.cpp)

include_directories(C:/Programming/boost)