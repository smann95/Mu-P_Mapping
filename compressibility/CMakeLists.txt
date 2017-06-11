cmake_minimum_required(VERSION 3.7)
project(compressibility)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        calculations.cpp
        compressibility.h
        data_handling.cpp
        main.cpp
        state_functions.cpp
        peng_robinson.cpp)

add_executable(compressibility ${SOURCE_FILES})