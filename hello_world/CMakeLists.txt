cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(sfml_hello)
add_executable(sfml_hello main.cpp)

target_link_libraries(sfml_hello sfml-graphics sfml-window sfml-system)
