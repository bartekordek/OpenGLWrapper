#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(openglwrapper_main)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add sub directories
add_subdirectory( deps )
add_subdirectory( deps/SDL2Wrapper )
add_subdirectory( libopenglwrapper )
add_subdirectory( libopenglwrapperPlaygroundApp )