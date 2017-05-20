cmake_minimum_required(VERSION 3.7)
project(compressibility)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp compressibility.h compressiblity.cpp calculations.cpp data_handling.cpp catch.h tests.cpp)
add_executable(compressibility ${SOURCE_FILES})