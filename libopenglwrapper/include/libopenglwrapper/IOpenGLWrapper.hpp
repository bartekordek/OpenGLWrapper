#pragma once

#include "libopenglwrapper/IObjectFactory.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"
#include "libopenglwrapper/IProgramFactory.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/Viewport.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/String.hpp"
#include "CUL/Graphics/Color.hpp"
#include "CUL/Log/ILogContainer.hpp"
#include "CUL/Graphics/IImageLoader.hpp"

NAMESPACE_BEGIN( LOGLW )

class IDebugOverlay;
class ProjectionData;

using String = CUL::String;

using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;

using CSize2Du = const Size2Du;
using CPos2Di = const Pos2Di;
using CMString = const String;
using IImageLoader = CUL::Graphics::IImageLoader;
using EmptyFunctionCallback = std::function<void()>;

class LIBOPENGLWRAPPER_API IOpenGLWrapper
{
public:
    IOpenGLWrapper();

    virtual void initialize() = 0;
    virtual void renderFrame() = 0;

    virtual void setBackgroundColor( const ColorS& color ) = 0;
    virtual void startRenderingLoop() = 0;
    virtual void stopRenderingLoop() = 0;
    virtual void onInitialize( const EmptyFunctionCallback& callback ) = 0;

    virtual IShaderFactory* getShaderFactory() = 0;
    virtual IObjectFactory* getObjectFactory() = 0;
    virtual IProgramFactory* getProgramFactory() = 0;
    virtual IImageLoader* getImageLoader() = 0;
    virtual IUtility* getUtility() = 0;
    virtual const Viewport& getViewport() const = 0;
    virtual ProjectionData* getProjectionData() = 0;

    virtual void addObjectToRender( IRenderable* renderable ) = 0;

    virtual CUL::CULInterface* getCul() = 0;
    virtual CUL::LOG::ILogger* getLoger() = 0;

    virtual void beforeFrame( const EmptyFunctionCallback& callback ) = 0;

    virtual void setProjection( const ProjectionData& rect ) = 0;
    virtual void setEyePos( const Pos3Df& pos ) = 0;
    virtual void setProjectionType( const ProjectionType type ) = 0;
    virtual void setViewport( const Viewport& viewport, const bool instant = false ) = 0;

    virtual void drawQuad( const bool draw = true ) = 0;

    virtual void clearModelViewEveryFrame( const bool enable ) = 0;

    virtual const ContextInfo& getContext() const = 0;

    virtual void drawDebugInfo( const bool enable ) = 0;

    virtual IDebugOverlay* getDebugOverlay() = 0;


    virtual ~IOpenGLWrapper();

protected:
private:
    IOpenGLWrapper( const IOpenGLWrapper& val ) = delete;
    IOpenGLWrapper( IOpenGLWrapper&& val ) = delete;
    IOpenGLWrapper& operator=( const IOpenGLWrapper& rhv ) = delete;
    IOpenGLWrapper& operator=( IOpenGLWrapper&& rhv ) = delete;
};

LIBOPENGLWRAPPER_API IOpenGLWrapper* createOpenGLWrapper( SDL2W::ISDL2Wrapper* sdl2w );

LIBOPENGLWRAPPER_API IOpenGLWrapper* getInstance();

NAMESPACE_END( LOGLW )