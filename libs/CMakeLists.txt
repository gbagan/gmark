# Register the path
set_property(GLOBAL PROPERTY LIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Add the libraries
add_subdirectory(argparse)
add_subdirectory(pugixml)
