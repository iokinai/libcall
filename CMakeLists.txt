cmake_minimum_required(VERSION 3.15)

include(cmake/project.cmake)

project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG REQUIRED)

add_library(libcall STATIC SHARED
    src/lib.cpp
)

target_include_directories(libcall PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include/magic_enum/include)