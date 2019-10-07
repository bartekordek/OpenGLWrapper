#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "OpenGLWrapperConcrete.hpp"

using namespace LOGLW;

IOpenGLWrapper* s_instance = nullptr;

IOpenGLWrapper* getInstance();

IOpenGLWrapper::IOpenGLWrapper()
{
}

IOpenGLWrapper::~IOpenGLWrapper()
{
}

IOpenGLWrapper* LOGLW::createOpenGLWrapper(
    SDL2W::IWindow* window,
    SDL2W::ISDL2Wrapper* sdl2w )
{
    s_instance = new OpenGLWrapperConcrete( window, sdl2w );
    return s_instance;
}

IOpenGLWrapper* getInstance()
{
    return s_instance;
}