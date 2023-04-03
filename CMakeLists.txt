cmake_minimum_required(VERSION 3.21)
project(veeam_test)

set(CMAKE_CXX_STANDARD 23)

set(HEADER_FILES
        include/error_codes.h
        include/synchro_logic.h
        )

set(SOURCE_FILES
        src/synchro_logic.cpp
        )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(synchronize ${HEADER_FILES} ${SOURCE_FILES} main.cpp)

target_include_directories(synchronize PUBLIC include)