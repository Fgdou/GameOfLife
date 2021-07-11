cmake_minimum_required(VERSION 3.17)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 14)

add_executable(GameOfLife src/main.cpp src/Screen.cpp src/Screen.h)