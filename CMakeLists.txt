cmake_minimum_required(VERSION 3.26)
project(yadro_assignment)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wc++20-extensions")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(yadro_assignment
  src/main.cpp
  src/parser.cpp
  src/simulation.cpp
)

target_include_directories(yadro_assignment PRIVATE include/)
