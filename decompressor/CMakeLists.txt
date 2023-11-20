cmake_minimum_required(VERSION 3.12)

project(
    decompressor
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(decompressor main.cpp)

target_compile_features(decompressor PUBLIC cxx_std_17)
set_target_properties(decompressor PROPERTIES CXX_EXTENSIONS OFF)

# Link against the static library
target_link_libraries(decompressor decompress_lib)