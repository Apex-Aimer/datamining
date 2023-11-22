cmake_minimum_required(VERSION 3.12)

project(
    assets_processor
    VERSION 1.0
    LANGUAGES CXX
)

# Bring the headers into the project
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(assets_processor ${SOURCES})

target_compile_features(assets_processor PUBLIC cxx_std_17)
set_target_properties(assets_processor PROPERTIES CXX_EXTENSIONS OFF)

# Link against the static library
# target_link_libraries(decompress_lib common_headers)