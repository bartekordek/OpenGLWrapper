#cmake -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.0.0)

project(openglWrapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXEC openglWrapper)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/SDL2Wrapper ${CMAKE_CURRENT_BINARY_DIR}/../deps/SDL2Wrapper)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps ${CMAKE_CURRENT_BINARY_DIR}/../deps)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../deps/SDL2Wrapper/deps/CUL/libcul/inc)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../deps/SDL2Wrapper/libsdl2wrapper/inc)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../deps/SDL2Wrapper/libsdl2wrapper/src)

if( MSVC )
    # warning level 4 and all warnings as errors
    add_compile_options( /W4 /WX )
    add_compile_options( /MP )
    #set_target_properties(SDL2 PROPERTIES
    #    COMPILE_FLAGS "/MP" # Adjust as needed
    #)
else()
    # lots of warnings and all warnings as errors
    #add_compile_options(-Wall -Wextra -pedantic -Werror)
    add_compile_options( -Wall -Wextra -pedantic -Werror )
    add_definitions( -Wfatal-errors )
endif()

add_library( ${EXEC} SHARED
include/libopenglwrapper/IProgram.cpp
src/IVBO.cpp
src/IObjectFactory.cpp
src/IOpenGLWrapper.cpp
src/IShader.cpp
src/IShaderFactory.cpp
src/IUtility.cpp
src/IUtilityUser.cpp
src/IVAO.cpp
src/OpenGLShaderFactory.cpp
src/OpenGLWrapperConcrete.cpp
src/Primitives/Rect.cpp
src/Primitives/Triangle.cpp
src/Primitives/TriangleImpl.cpp
src/ProgramConcrete.cpp
src/ProjectionData.cpp
src/ShaderConcrete.cpp
src/ShaderRegister.cpp
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
src/ShaderRegister.hpp
src/UtilConcrete.hpp
src/VAOConcrete.hpp
src/VAOOpengl.hpp
src/VBOConcrete.hpp
)

if( MSVC )
    target_link_libraries( ${EXEC} LOGLW-AdditionalDeps sdl2wrapper cul AdditionalDeps )
else()
    target_link_libraries( ${EXEC} LOGLW-AdditionalDeps sdl2wrapper cul AdditionalDeps stdc++fs  )
endif()

target_include_directories(${EXEC} PUBLIC include)
target_include_directories(${EXEC} PRIVATE src)
