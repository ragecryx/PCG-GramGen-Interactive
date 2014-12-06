cmake_minimum_required (VERSION 2.6)
project(PCGGramGen-Interactive)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/DIST")
set(CMAKE_CXX_FLAGS "-std=c++11 -static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")

# Options

option(DEBUG
       "Enable debug printing." OFF)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)

# Config header
configure_file(
    "${PROJECT_SOURCE_DIR}/src/gramgen_int_conf.h.in"
    "${PROJECT_BINARY_DIR}/gramgen_int_conf.h"
)

# Sources and Building
include_directories(
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/deps/include"
)

link_directories(
    "${PROJECT_SOURCE_DIR}/deps/lib"
)

file(GLOB SourceFiles
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(gramgen_interactive ${SourceFiles})
target_link_libraries(gramgen_interactive PCGGram)

# Installation
install(TARGETS gramgen_interactive DESTINATION bin)
