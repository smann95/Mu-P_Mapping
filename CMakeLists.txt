cmake_minimum_required(VERSION 3.7)

project(Mu-P_Mapping)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/data_handling.cpp
        src/peng_robinson.cpp
        src/Reference_Data.cpp
        src/compressibility.h)

set(Boost_USE_STATIC_LIBS OFF)
set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USER_STATIC_RUNTIME OFF)
find_package(Boost 1.62 COMPONENTS filesystem system)

include_directories(${Boost_INCLUDE_DIRS})


# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR tests/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_executable(mapMu-P src/main.cpp ${SOURCE_FILES})
target_link_libraries(mapMu-P ${Boost_LIBRARIES})

add_executable(mapMu-Ptests tests/tests_main.cpp)
target_link_libraries(mapMu-Ptests Catch ${Boost_LIBRARIES})

