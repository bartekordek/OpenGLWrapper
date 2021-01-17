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
    #set_target_properties(SDL2 PROPERTIES
    #    COMPILE_FLAGS "/MP" # Adjust as needed
    #)
else()
    # lots of warnings and all warnings as errors
    add_compile_options( -Wall -Wextra -pedantic -Werror )
    add_definitions( -Wfatal-errors )
endif()

message( "[openglwrapper lib] Setting CMAKE_ARCHIVE_OUTPUT_DIRECTORY to: ${OGLW_OUTPUT_DIR}" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OGLW_OUTPUT_DIR} )

add_library( ${EXEC} SHARED
include/libopenglwrapper/IProgram.cpp
src/IVBO.cpp
src/IObjectFactory.cpp
src/IOpenGLWrapper.cpp
src/IShader.cpp
src/IShaderFactory.cpp
src/IUtility.cpp
src/IUtilityUser.cpp
src/ITexture.cpp
src/IVAO.cpp
src/OpenGLShaderFactory.cpp
src/OpenGLWrapperConcrete.cpp
src/Primitives/Rect.cpp
src/Primitives/Triangle.cpp
src/Primitives/TriangleImpl.cpp
src/ProgramConcrete.cpp
src/ProjectionData.cpp
src/ShaderConcrete.cpp
src/TextureConcrete.cpp
src/UtilConcrete.cpp
src/VAOConcrete.cpp
src/VAOOpengl.cpp
src/VBOConcrete.cpp
src/Viewport.cpp
)

#set_target_properties(cul PROPERTIES PUBLIC_HEADER
set( HEADERS_PUBLIC
include/libopenglwrapper/IDebugOverlay.hpp
include/libopenglwrapper/IMovable.hpp
include/libopenglwrapper/Import.hpp
include/libopenglwrapper/IObject.hpp
include/libopenglwrapper/IObjectFactory.hpp
include/libopenglwrapper/IOpenGLWrapper.hpp
include/libopenglwrapper/IProgram.hpp
include/libopenglwrapper/IProgramFactory.hpp
include/libopenglwrapper/IRenderable.hpp
include/libopenglwrapper/IShader.hpp
include/libopenglwrapper/IShaderFactory.hpp
include/libopenglwrapper/ITexture.hpp
include/libopenglwrapper/IUtility.hpp
include/libopenglwrapper/IUtilityUser.hpp
include/libopenglwrapper/IVAO.hpp
include/libopenglwrapper/IVBO.hpp
include/libopenglwrapper/IVertexArray.hpp
include/libopenglwrapper/IVertexBuffer.hpp
include/libopenglwrapper/Primitives/IRect.hpp
include/libopenglwrapper/Primitives/Triangle.hpp
include/libopenglwrapper/ProjectionData.hpp
include/libopenglwrapper/ViewPort.hpp
include/libopenglwrapper/Viewport.hpp
src/OpenGLShaderFactory.hpp
src/ImportFreeglut.hpp
src/ImportImgui.hpp
src/IMPORT_glew.hpp
src/MatrixStack.hpp
src/OpenGLWrapperConcrete.hpp
src/Primitives/TriangleImpl.hpp
src/ProgramConcrete.hpp
src/ShaderConcrete.hpp
src/TextureConcrete.hpp
src/UtilConcrete.hpp
src/VAOConcrete.hpp
src/VAOOpengl.hpp
src/VBOConcrete.hpp
)


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
#
    #message( "[openglwrapper lib] glew lib: $<glew>" )
#
    #add_custom_command(TARGET ${EXEC} POST_BUILD
    #    COMMAND "xcopy" "${OGLW_DLLS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
    #    COMMAND "xcopy" "${OGLW_PDBS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
#
    #    COMMAND "xcopy" "${OGLW_BIN_DLLS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
    #    COMMAND "xcopy" "${OGLW_BIN_PDBS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
#
    #    COMMAND "xcopy" "${SDLW_DLLS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
    #    COMMAND "xcopy" "${SDLW_PDBS}" "${OGLW_TESTS_RUN_DIR_WINDOWS}" "/y" "/f"
    #)

    add_custom_command(TARGET ${EXEC} POST_BUILD
        COMMAND ${OGLW_ROOT_WINDOWS}\\Copy.bat ${OGLW_OUTPUT_DIR_WINDOWS} ${OGLW_TESTS_RUN_DIR_WINDOWS}
        COMMAND ${OGLW_ROOT_WINDOWS}\\Copy.bat ${OGLW_DEPS_ROOT_WINDOWS} ${OGLW_TESTS_RUN_DIR_WINDOWS}
    )
endif()

target_include_directories(${EXEC} PUBLIC include)
target_include_directories(${EXEC} PRIVATE src)