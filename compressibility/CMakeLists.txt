project(compressibility)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        compressibility.h
        data_handling.cpp
        main.cpp
        peng_robinson.cpp Reference_Data.cpp)

add_executable(compressibility ${SOURCE_FILES})
