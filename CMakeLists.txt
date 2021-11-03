cmake_minimum_required(VERSION 3.16)
project(STM32_Miosix_GameOfLife)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(STM32_Miosix_GameOfLife
        main.cpp GameOfLife.cpp GameOfLife.h)
