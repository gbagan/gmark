cmake_minimum_required(VERSION 2.8)

# Project metadata
project(gmark-interface)

# Sources
file(GLOB SOURCES *.cpp)

# Executables
add_executable(gmark-interface ${SOURCES})

# Find the headers folders
get_property(libs_path GLOBAL PROPERTY LIBS_PATH)
target_include_directories(gmark-interface PRIVATE ${libs_path})

# Add libraries
target_link_libraries(gmark-interface PUBLIC cpp-mini-argparse pugixml)

install(TARGETS gmark-interface DESTINATION bin)
