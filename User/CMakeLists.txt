cmake_minimum_required(VERSION 3.29)

project(project)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(User start.cpp)

target_sources(User PRIVATE error_code.cpp Client.cpp)

include_directories(C:/Programming/boost)