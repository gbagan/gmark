cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-gen)

# Sources
file(GLOB SOURCES *.cpp monStaGen/*.cpp)

# Executables
add_executable(gmark-gen ${SOURCES})
install(TARGETS gmark-gen DESTINATION bin)
