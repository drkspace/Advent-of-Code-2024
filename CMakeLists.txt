cmake_minimum_required(VERSION 3.22)
project(avoc2024)

set(CMAKE_CXX_COMPILER g++-14)
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Werror)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cppitertools/cppitertools)

add_executable(avoc2024_day1 day1.cpp)
add_executable(avoc2024_day2 day2.cpp)
add_executable(avoc2024_day3 day3.cpp)
add_executable(avoc2024_day4 day4.cpp)
add_executable(avoc2024_day5 day5.cpp)
add_executable(avoc2024_day6 day6.cpp)
add_executable(avoc2024_day7 day7.cpp)
add_executable(avoc2024_day8 day8.cpp)
add_executable(avoc2024_day9 day9.cpp
        easyTimer.h)
add_executable(avoc2024_day10 day10.cpp
        easyTimer.h)
add_executable(avoc2024_day11 day11.cpp
        easyTimer.h)
add_executable(avoc2024_day12 day12.cpp
        easyTimer.h)
add_executable(avoc2024_day13 day13.cpp
        easyTimer.h)

add_executable(avoc2024_day14 day14.cpp
        easyTimer.h)
add_executable(avoc2024_day15 day15.cpp
        easyTimer.h)

add_executable(avoc2024_day16 day16.cpp
        easyTimer.h)
add_executable(avoc2024_day17 day17.cpp
        easyTimer.h)
add_executable(avoc2024_day18 day18.cpp
        easyTimer.h)
add_executable(avoc2024_day19 day19.cpp
        easyTimer.h utils/utils.h)
add_executable(avoc2024_day20 day20.cpp
        easyTimer.h utils/utils.h)
add_executable(avoc2024_day21 day21.cpp
        easyTimer.h utils/utils.h)
add_executable(avoc2024_day22 day22.cpp
        easyTimer.h utils/utils.h)
add_executable(avoc2024_day23 day23.cpp
        easyTimer.h utils/utils.h
        utils/point.h)
add_executable(avoc2024_day24 day24.cpp
        easyTimer.h utils/utils.h
        utils/point.h cppitertools/cppitertools/combinations.hpp)
add_executable(avoc2024_day25 day25.cpp
        easyTimer.h utils/utils.h
        utils/point.h)