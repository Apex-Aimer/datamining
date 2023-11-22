cmake_minimum_required(VERSION 3.12)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(datamining)

# Include directories for the common header
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

add_subdirectory(decompress)
add_subdirectory(decompressor)
add_subdirectory(assets_processor)