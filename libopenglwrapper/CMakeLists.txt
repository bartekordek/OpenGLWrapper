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
message( "[openglwrapper lib] DONE" )

message( "[openglwrapper lib] 00 CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
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

message( "[openglwrapper lib] Setting CMAKE_ARCHIVE_OUTPUT_DIRECTORY to: ${OGLW_OUTPUT_DIR}" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )

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
freeglut
${OPENGL_LIBRARIES}
${OPENGL_LIBRARY}
)

message( "[openglwrapper lib] DEPENDENCIES: ${DEPENDENCIES}" )

if( MSVC )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs  )
endif()

if (MSVC)
    CMakeToWindow( ${OGLW_ROOT} OGLW_ROOT_WINDOWS )
    CMakeToWindow( ${OGLW_OUTPUT_DIR} OGLW_OUTPUT_DIR_WINDOWS )
    CMakeToWindow( ${OGLW_DEPS_ROOT} OGLW_DEPS_ROOT_WINDOWS )

    CmakeToWindow( "${OGLW_TESTS_RUN_DIR}" OGLW_TESTS_RUN_DIR_WINDOWS )
    CmakeToWindow( "${OGLW_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/*.dll" OGLW_DLLS )
    CmakeToWindow( "${OGLW_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/*.pdb" OGLW_PDBS )

    CmakeToWindow( "${OGLW_OUTPUT_DIR}/bin/${CMAKE_BUILD_TYPE}/*.dll" OGLW_BIN_DLLS )
    CmakeToWindow( "${OGLW_OUTPUT_DIR}/bin/${CMAKE_BUILD_TYPE}/*.pdb" OGLW_BIN_PDBS )

    CMakeToWindow( $ENV{SDL2W_TESTS_RUN_DIR}/*.dll SDLW_DLLS )
    CMakeToWindow( $ENV{SDL2W_TESTS_RUN_DIR}/*.pdb SDLW_PDBS )

    CMakeToWindow( ${OGLW_OUTPUT_DIR}/*.dll INPUT_DIR_DLL )
    CMakeToWindow( ${OGLW_OUTPUT_DIR}/*.pdb INPUT_DIR_PDB )

    set( CopyCommand ${OGLW_DEPS_ROOT_WINDOWS}\\helpers\\CopyLibraryBinaryAndSymbolsRecursive.bat )

    add_custom_command(TARGET ${EXEC} POST_BUILD
        COMMAND ${CopyCommand} ${OGLW_OUTPUT_DIR_WINDOWS} ${OGLW_TESTS_RUN_DIR_WINDOWS}
        COMMAND ${CopyCommand} ${OGLW_DEPS_ROOT_WINDOWS} ${OGLW_TESTS_RUN_DIR_WINDOWS}
    )
endif()

target_include_directories(${EXEC} PUBLIC include)
target_include_directories(${EXEC} PRIVATE src)
