cmake_minimum_required(VERSION 3.12)

project(
    decompress_lib
    VERSION 1.0
    LANGUAGES CXX
)

# Bring the headers into the project
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_library(decompress_lib STATIC ${SOURCES})

# Specify C++17 standard
target_compile_features(decompress_lib PUBLIC cxx_std_17)
set_target_properties(decompress_lib PROPERTIES CXX_EXTENSIONS OFF)

# Include directories for the target
target_include_directories(decompress_lib PUBLIC include)
