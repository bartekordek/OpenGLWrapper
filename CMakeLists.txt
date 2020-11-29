#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(libopenglwrapperPlaygroundApp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(EXEC libopenglwrapperPlaygroundApp)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/libopenglwrapper )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/libopenglwrapper/include)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2Wrapper/libsdl2wrapper/inc )

message( "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}" )

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    add_compile_options( /MP )
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    add_definitions(-Wfatal-errors)
endif()

include_directories(${EXEC})
file( GLOB SOURCES ${EXEC}/*.cpp )

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


if( MSVC )
    target_link_libraries( ${EXEC} openglWrapper LOGLW-AdditionalDeps cul AdditionalDeps )
else()
    target_link_libraries( ${EXEC} openglWrapper LOGLW-AdditionalDeps cul AdditionalDeps stdc++fs  )
endif()

target_include_directories(${EXEC} PRIVATE inc)
target_include_directories(${EXEC} PRIVATE src)

if( MSVC )
    set_property(TARGET ${EXEC} PROPERTY "VS_DEBUGGER_WORKING_DIRECTORY" "${CMAKE_SOURCE_DIR}/runDir")
endif()

#add_custom_command(TARGET ${EXEC} POST_BUILD       # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#    "${PROJECT_SOURCE_DIR}/libs/test.dll"      # <--this is in-file
#        $<TARGET_FILE_DIR:${EXEC}>)                 # <--this is out-file path
