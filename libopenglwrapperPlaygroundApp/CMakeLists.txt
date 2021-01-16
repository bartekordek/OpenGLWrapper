#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

get_filename_component( basename ${CMAKE_CURRENT_SOURCE_DIR} NAME )
project(${basename} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(EXEC ${PROJECT_NAME})

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    add_compile_options( /MP )
    set( OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Build-Windows )
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    add_definitions(-Wfatal-errors)
    set( OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Build-Linux )
endif()

message( "[PlaygroundApp] Setting CMAKE_ARCHIVE_OUTPUT_DIRECTORY to: ${OUTPUT_DIR}" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR} )

file( GLOB SOURCES *.cpp )

add_executable(${EXEC} ${SOURCES})

if (MSVC)
    set_target_properties(
        ${EXEC}
        PROPERTIES
            OUTPUT_NAME ${EXEC}
            SUFFIX ".exe"
    )
else()
    set_target_properties(
        ${EXEC}
        PROPERTIES
            OUTPUT_NAME ${EXEC}
            SUFFIX ".bin"
    )
endif()

IF(NOT APPLE AND NOT MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF()

find_package(OpenGL REQUIRED)

set( DEPENDENCIES
openglWrapper
LOGLW-AdditionalDeps
cul
AdditionalDeps
${GLEW_LIBRARIES}
${GLFW3_LIBRARIES}
freeglut
${OPENGL_LIBRARIES}
${OPENGL_LIBRARY}
)

#message( "DEPENDENCIES: ${DEPENDENCIES}" )

if( MSVC )
    target_link_libraries( ${EXEC} ${DEPENDENCIES} )
else()
    target_link_libraries( ${EXEC} ${DEPENDENCIES} stdc++fs  )
endif()

target_include_directories(${EXEC} PRIVATE inc)
target_include_directories(${EXEC} PRIVATE src)

if( MSVC )
    set_property(TARGET ${EXEC} PROPERTY "VS_DEBUGGER_WORKING_DIRECTORY" "${CMAKE_SOURCE_DIR}/runDir")
endif()

#file(COPY GET_RUNTIME_DEPENDENCIES ${OUTPUT_DIR})

#add_custom_command(TARGET ${EXEC} POST_BUILD       # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#    "${PROJECT_SOURCE_DIR}/libs/test.dll"      # <--this is in-file
#        $<TARGET_FILE_DIR:${EXEC}>)                 # <--this is out-file path
