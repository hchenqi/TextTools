cmake_minimum_required(VERSION 4.0)
project(TextTools CXX)

find_package(harfbuzz CONFIG REQUIRED)

add_subdirectory("../WndDesign" WndDesign)

add_executable(TextTools src/main.cpp)
target_link_libraries(TextTools PRIVATE harfbuzz::harfbuzz)
target_link_libraries(TextTools PRIVATE WndDesign)
