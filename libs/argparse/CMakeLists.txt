# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the source files
add_library (cpp-mini-argparse STATIC ${SOURCES} ${HEADERS})
