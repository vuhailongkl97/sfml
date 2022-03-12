cmake_minimum_required(VERSION 2.4.0)

project(observer)

# Find source files
file(GLOB SOURCES src/*.cpp)

# Include header files
include_directories(inc)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
file(GLOB HEADERS include/*.h)

set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -header-filter=.;
  -warnings-as-errors=*;)
