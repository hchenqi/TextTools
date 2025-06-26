cmake_minimum_required(VERSION 4.0)
project(TextTools CXX)

find_package(harfbuzz CONFIG REQUIRED)

add_executable(TextTools main.cpp)
target_link_libraries(TextTools PRIVATE harfbuzz::harfbuzz)
