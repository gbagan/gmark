cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-translate)

# Sources
file(GLOB SOURCES *.cpp)

# Executable
add_executable(gmark-translate ${SOURCES})

# Find the headers folders
get_property(libs_path GLOBAL PROPERTY LIBS_PATH)
target_include_directories(gmark-translate PRIVATE ${libs_path})

# Add libraries
target_link_libraries(gmark-translate PUBLIC cpp-mini-argparse pugixml)

install(TARGETS gmark-translate DESTINATION bin)
