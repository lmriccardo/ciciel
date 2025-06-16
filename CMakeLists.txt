cmake_minimum_required(VERSION 3.19)

project(LamaLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)

add_compile_options(-Wall -Wextra -pedantic -lrt -lpthread)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0)
    # add_compile_options(-fdiagnostics-color=always)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include( FindCursesSupport )
include( InstallUtf8Proc )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(src)
enable_testing()
add_subdirectory(test)