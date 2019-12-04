#pragma once

#include "libopenglwrapper/IObjectFactory.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"
#include "libopenglwrapper/IProgramFactory.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics//Size2D.hpp"
#include "CUL/MyString.hpp"
#include "CUL/Graphics/Color.hpp"
#include "CUL/Log/ILogContainer.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size2Du = CUL::Graphics::Size2Du;
using Pos2Di = CUL::Graphics::Pos2Di;
using MString = CUL::MyString;

using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;

using CSize2Du = const Size2Du;
using CPos2Di = const Pos2Di;
using CMString = const MString;

class LIBOPENGLWRAPPER_API IOpenGLWrapper
{
public:
    IOpenGLWrapper();
    virtual ~IOpenGLWrapper();

    virtual void initialize() = 0;
    virtual void renderFrame() = 0;

    virtual void setBackgroundColor( const ColorS& color ) = 0;
    virtual void startRenderingLoop() = 0;
    virtual void stopRenderingLoop() = 0;
    virtual void onInitialize( const std::function<void()>& callback ) = 0;

    virtual IShaderFactory* getShaderFactory() = 0;
    virtual IObjectFactory* getObjectFactory() = 0;
    virtual IProgramFactory* getProgramFactory() = 0;

    virtual CUL::LOG::ILogger* getLoger() = 0;

    virtual void beforeFrame( const std::function<void()>& callback ) = 0;


protected:
private:
    IOpenGLWrapper( const IOpenGLWrapper& val ) = delete;
    IOpenGLWrapper& operator=( const IOpenGLWrapper& rhv ) = delete;

};

LIBOPENGLWRAPPER_API IOpenGLWrapper* createOpenGLWrapper( SDL2W::ISDL2Wrapper* sdl2w );

LIBOPENGLWRAPPER_API IOpenGLWrapper* getInstance();

NAMESPACE_END( LOGLW )