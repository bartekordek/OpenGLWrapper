#pragma once

#include "libopenglwrapper/IObjectFactory.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/Pos2D.hpp"
#include "CUL/Size2D.hpp"
#include "CUL/MyString.hpp"
#include "CUL/Color.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size2Du = CUL::Size2Du;
using Pos2Di = CUL::Pos2Di;
using MString = CUL::MyString;

using ColorS = CUL::ColorS;
using ColorE = CUL::ColorE;

using CSize2Du = const Size2Du;
using CPos2Di = const Pos2Di;
using CMString = const MString;

class LIBOPENGLWRAPPER_API IOpenGLWrapper
{
public:
    IOpenGLWrapper();
    virtual ~IOpenGLWrapper();

    virtual void renderFrame() = 0;
    virtual void setBackgroundColor( const ColorS& color ) = 0;
    virtual void test() = 0;
    virtual void startRenderingLoop() = 0;
    virtual void stopRenderingLoop() = 0;
    virtual void onInitialize( const std::function<void()>& callback ) = 0;

    virtual IShaderFactory* getShaderFactory() = 0;
    virtual IObjectFactory* getObjectFactory() = 0;

protected:
private:
    IOpenGLWrapper( const IOpenGLWrapper& val ) = delete;
    IOpenGLWrapper& operator=( const IOpenGLWrapper& rhv ) = delete;

};

LIBOPENGLWRAPPER_API IOpenGLWrapper* createOpenGLWrapper(
    SDL2W::IWindow* window,
    SDL2W::ISDL2Wrapper* sdl2w );

NAMESPACE_END( LOGLW )