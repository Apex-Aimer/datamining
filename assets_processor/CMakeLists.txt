cmake_minimum_required(VERSION 3.12)

project(
    assets_processor
    VERSION 1.0
    LANGUAGES CXX
)

# Bring the headers into the project
include_directories(include)

# Specify library directory
link_directories(../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/lib/Mac)

file(GLOB SOURCES "src/*.cpp")

add_executable(assets_processor ${SOURCES})

# Specify include directory if needed
target_include_directories(
    assets_processor 
    PUBLIC 
    ../OodleUE/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.11/include
)

target_compile_features(assets_processor PUBLIC cxx_std_17)
set_target_properties(assets_processor PROPERTIES CXX_EXTENSIONS OFF)

# Specify static libraries to link with
target_link_libraries(assets_processor PUBLIC oo2coremac64)