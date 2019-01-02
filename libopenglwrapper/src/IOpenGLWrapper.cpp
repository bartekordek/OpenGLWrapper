#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "OpenGLWrapperConcrete.hpp"

using namespace LOGLW;

IOpenGLWrapper::IOpenGLWrapper( void )
{
}

IOpenGLWrapper::~IOpenGLWrapper( void )
{
}

IOpenGLWrapper* LOGLW::createOpenGLWrapper(
    SDL2W::IWindow* window,
    SDL2W::ISDL2Wrapper* sdl2w )
{
    return new OpenGLWrapperConcrete( window, sdl2w );
}