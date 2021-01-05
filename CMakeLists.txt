#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(openglwrapper_main)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add sub directories
add_subdirectory( deps/SDL2Wrapper )
add_subdirectory( deps )

#add_subdirectory( deps/cegui )

#BUILD_UTILS
#option(BUILD_UTILS "No utils" OFF)
add_subdirectory( deps/freeglut-3.2.1 )
add_subdirectory( deps/glew-2.1.0/build/cmake )
add_subdirectory( deps/glfw-repo )

add_subdirectory( deps/SDL2Wrapper/deps/CUL/deps/glm )

option(RENDER_OPENGL_CORE "Enable Opengl" ON)
add_subdirectory( deps/GWork )

add_subdirectory( libopenglwrapper )
add_subdirectory( libopenglwrapperPlaygroundApp )
