cmake_minimum_required(VERSION 3.12)

project(
    extractor
    VERSION 1.0
    LANGUAGES CXX
)

# Specify library directory
link_directories(../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/lib/Mac)

add_executable(extractor main.cpp)

# Specify include directory if needed
target_include_directories(
    extract_assets_lib
    PUBLIC 
    ../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/include
)

target_compile_features(extractor PUBLIC cxx_std_17)
set_target_properties(extractor PROPERTIES CXX_EXTENSIONS OFF)

# Link against the static library
target_link_libraries(extractor decompress_lib)
target_link_libraries(extractor extract_assets_lib)