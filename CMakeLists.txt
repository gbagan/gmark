cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gMark)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
add_subdirectory(src)
add_subdirectory(src/queryinterface)
add_subdirectory(src/querytranslate)
