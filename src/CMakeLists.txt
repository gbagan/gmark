cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-gen)

# Sources
file(GLOB SOURCES *.cpp monStaGen/*.cpp)

# Executable
add_executable(gmark-gen ${SOURCES})

# Find the headers folders
get_property(libs_path GLOBAL PROPERTY LIBS_PATH)
target_include_directories(gmark-gen PRIVATE ${libs_path})

# Add libraries
target_link_libraries(gmark-gen PUBLIC cpp-mini-argparse pugixml)

install(TARGETS gmark-gen DESTINATION bin)
