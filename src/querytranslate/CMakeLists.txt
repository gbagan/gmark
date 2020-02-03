cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-translate)

# Sources
file(GLOB SOURCES *.cpp)

# Executables
add_executable(gmark-translate ${SOURCES})
install(TARGETS gmark-translate DESTINATION bin)
