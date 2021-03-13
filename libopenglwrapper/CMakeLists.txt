#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(openglWrapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXEC ${PROJECT_NAME})

include_directories( SYSTEM ${GLM_INCLUDE_DIR} )
include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
include_directories( SYSTEM ${GLFW_INCLUDE_DIR} )
include_directories( SYSTEM ${FREEGLUT_INCLUDE_DIR} )

message( "[openglwrapper lib] GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}" )
message( "[openglwrapper lib] GLEW_HOME: ${GLEW_HOME}" )

message( "[openglwrapper lib] PRINTING:" )
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${dir}")
endforeach()
find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( freeglut REQUIRED )
find_package( GLFW REQUIRED )

if( MSVC )
    # warning level 4 and all warnings as errors
    add_compile_options( /W4 /WX )
    add_compile_options( /MP )
else()
    # lots of warnings and all warnings as errors
    add_compile_options( -Wall -Wextra -pedantic -Werror )
    add_definitions( -Wfatal-errors )
endif()

file( GLOB_RECURSE SOURCES include/*.hpp src/*.hpp src/*.cpp )
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} )
add_library( ${EXEC} SHARED ${SOURCES} )

set( DEPENDENCIES
LOGLW-AdditionalDeps
sdl2wrapper
cul
glew
AdditionalDeps
${GLEW_LIBRARIES}
${GLFW3_LIBRARIES}
${GLFW_LIBRARIES}
freeglut_static
${OPENGL_LIBRARIES}
${OPENGL_LIBRARY}
)

message( "[openglwrapper lib] DEPENDENCIES: ${DEPENDENCIES}" )

if( MSVC )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs  )
endif()

target_include_directories(${EXEC} PUBLIC include)
target_include_directories(${EXEC} PRIVATE src)
