cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(sfml_hello)

find_program(CLANG_TIDY clang-tidy)
message("use" ${CLANG_TIDY})
set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -header-filter=.;
  -warnings-as-errors=*;)

set (CMAKE_CXX_STANDARD 11)
message(${CMAKE_CURRENT_SOURCE_DIR})
add_compile_options(-DIMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../image")

add_executable(sfml_hello src/main.cpp src/Observer.cpp src/Orbit.cpp src/Star.cpp)

target_include_directories(sfml_hello PUBLIC inc/)
target_link_libraries(sfml_hello sfml-graphics sfml-window sfml-system)
