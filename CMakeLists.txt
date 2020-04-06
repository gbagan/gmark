cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gMark)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ensure we have the same output directory for Visual Studio & others
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Libraries
add_subdirectory(libs)

# Sources
add_subdirectory(src)
add_subdirectory(src/queryinterface)
add_subdirectory(src/querytranslate)
