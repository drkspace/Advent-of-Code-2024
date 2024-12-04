cmake_minimum_required(VERSION 3.22)
project(avoc2024)

set(CMAKE_CXX_COMPILER g++-14)
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Werror)

add_executable(avoc2024_day1 day1.cpp)
add_executable(avoc2024_day2 day2.cpp)
add_executable(avoc2024_day3 day3.cpp)
add_executable(avoc2024_day4 day4.cpp)
