cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-interface)

# Sources
file(GLOB SOURCES *.cpp)

# Executables
add_executable(gmark-interface ${SOURCES})
install(TARGETS gmark-interface DESTINATION bin)
