cmake_minimum_required(VERSION 3.20)

project(CIOS)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    cios
    src/main.cpp
    src/cios.cpp
    src/terminal.cpp
    src/config.cpp
    src/boot.cpp
)

target_include_directories(
    cios
    PRIVATE
    include
)
