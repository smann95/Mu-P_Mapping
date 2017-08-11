cmake_minimum_required(VERSION 3.7)

project(Mu-P_Mapping)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/compressibility.h
        src/data_handling.cpp
        src/main.cpp
        src/peng_robinson.cpp
        src/Reference_Data.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USER_STATIC_RUNTIME OFF)
find_package(Boost 1.62 COMPONENTS filesystem system)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(mapMu-P ${SOURCE_FILES})
target_link_libraries(mapMu-P ${Boost_LIBRARIES})
