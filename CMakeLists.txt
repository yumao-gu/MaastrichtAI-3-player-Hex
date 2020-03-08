cmake_minimum_required(VERSION 3.15)
project(HEX)

set(CMAKE_CXX_STANDARD 14)

add_executable(HEX main.cpp include/HEX.h include/BOARD.h include/PLAYER.h include/GRAPH.h include/PLAY.h)