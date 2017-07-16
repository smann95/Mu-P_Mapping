cmake_minimum_required(VERSION 3.7)
project(compressibility)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        compressibility.h
        data_handling.cpp
        main.cpp
        peng_robinson.cpp Reference_Data.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62 COMPONENTS filesystem )

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(compressibility ${SOURCE_FILES})
    target_link_libraries(compressibility ${Boost_LIBRARIES})
endif()


