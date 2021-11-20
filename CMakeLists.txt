cmake_minimum_required(VERSION 3.16)
project(STM32_Miosix_GameOfLife)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(.)

add_executable(STM32_Miosix_GameOfLife
        starter.cpp Terminal.cpp GameOfLife.cpp Controller.cpp Prompt.cpp)
