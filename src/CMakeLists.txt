cmake_minimum_required(VERSION 3.15)
project(Cells)

set(CMAKE_CXX_STANDARD 11)

add_executable(Cells main.cpp CellularAutomata.cpp Cell.cpp Cell.h CellularAutomata.h Individual.h Individual.cpp State.cpp State.h CFG.cpp CFG.h PDA.cpp PDA.h)