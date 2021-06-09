#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "OpenGLWrapperConcrete.hpp"

#include "SDL2Wrapper/WindowData.hpp"

using namespace LOGLW;

IOpenGLWrapper* IOpenGLWrapper::s_instance = nullptr;


IOpenGLWrapper::IOpenGLWrapper()
{
}

IOpenGLWrapper* IOpenGLWrapper::createOpenGLWrapper( SDL2W::ISDL2Wrapper* sdl2w, bool legacy )
{
    s_instance = new OpenGLWrapperConcrete( sdl2w, legacy );
    return s_instance;
}

IOpenGLWrapper* IOpenGLWrapper::createOpenGLWrapper(
    bool legacy,
    const CUL::Graphics::Pos2Di& pos,
    const SDL2W::WindowSize& winSize,
    const String& configPath,
    const String& winName,
    const String& renderername )
{
    SDL2W::WindowData windowData;
    windowData.name = winName;
    windowData.pos = pos;
    windowData.currentRes = winSize;
    windowData.rendererName = renderername;

    auto sdlWrap = SDL2W::ISDL2Wrapper::createSDL2Wrapper();
    sdlWrap->init( windowData, configPath );

    s_instance = new OpenGLWrapperConcrete( sdlWrap, legacy );
    return s_instance;
}

IOpenGLWrapper* IOpenGLWrapper::getInstance()
{
    return s_instance;
}

IOpenGLWrapper::~IOpenGLWrapper()
{
}