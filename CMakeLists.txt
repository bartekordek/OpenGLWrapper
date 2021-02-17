#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(openglwrapper_main)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( OGLW_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

if( MSVC )
    set( BUILD_ROOT_POSTFIX Build-Windows )
else()
    set( BUILD_ROOT_POSTFIX Build-Linux )
endif()

set( OGLW_OUTPUT_DIR ${OGLW_ROOT}/${BUILD_ROOT_POSTFIX} )

set( OGLW_TESTS_RUN_DIR ${OGLW_ROOT}/runDir )
set( OGLW_DEPS_ROOT ${OGLW_ROOT}/deps )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2Wrapper/deps/CUL/cmake_modules" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/deps" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )
message( "[openglwarpper] CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}" )

# Add sub directories
add_subdirectory( deps/SDL2Wrapper )
add_subdirectory( deps )
set( FREEGLUT_HOME ${OGLW_ROOT}/deps/freeglut-3.2.1 )
set( ENV{FREEGLUT_HOME} ${FREEGLUT_HOME} )
set( FREEGLUT_INCLUDE_DIR ${FREEGLUT_HOME}/include )
set( FREEGLUT_BUILD_DEMOS "Build demos." OFF )
add_subdirectory( ${FREEGLUT_HOME} )

set( GLM_INCLUDE_DIR $ENV{GLM_INCLUDE_DIR} )
set( GLM_INCLUDE_DIRS $ENV{GLM_INCLUDE_DIRS} )

set( GLEW_HOME ${CMAKE_CURRENT_SOURCE_DIR}/deps/glew-2.1.0 )
set( ENV{GLEW_HOME} ${GLEW_HOME} )
set( GLFW_HOME ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw-repo )
set( GLEW_INCLUDE_DIR ${GLEW_HOME}/include  )
set( GLEW_INCLUDE_DIRS ${GLEW_HOME}/include )
set( GLFW_INCLUDE_DIR ${GLFW_HOME}/include )

list(APPEND CMAKE_MODULE_PATH ${GLEW_HOME} )
add_subdirectory( ${GLEW_HOME}/build/cmake )
add_subdirectory( ${GLFW_HOME} )

option(RENDER_OPENGL_CORE "Enable Opengl" ON)

add_subdirectory( libopenglwrapper )
add_subdirectory( libopenglwrapperPlaygroundApp )
