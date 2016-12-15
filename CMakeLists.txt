cmake_minimum_required(VERSION 3.6)
project(sc2_ai VERSION 0.1 LANGUAGES CXX)

include_directories(ext/bwapi/include)

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(test)