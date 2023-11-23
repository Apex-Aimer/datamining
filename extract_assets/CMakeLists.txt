cmake_minimum_required(VERSION 3.12)

project(
    extract_assets_lib
    VERSION 1.0
    LANGUAGES CXX
)

# Bring the headers into the project
include_directories(include)

# Specify library directory
link_directories(../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/lib/Mac)

file(GLOB SOURCES "src/*.cpp")

add_library(extract_assets_lib STATIC ${SOURCES})

# Specify include directory if needed
target_include_directories(
    extract_assets_lib
    PUBLIC 
    ../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/include
)

target_compile_features(extract_assets_lib PUBLIC cxx_std_17)
set_target_properties(extract_assets_lib PROPERTIES CXX_EXTENSIONS OFF)

# Specify static libraries to link with
target_link_libraries(extract_assets_lib PUBLIC oo2coremac64)

# Include directories for the target to be available outside
target_include_directories(decompress_lib PUBLIC include)