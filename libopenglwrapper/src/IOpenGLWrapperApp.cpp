#include "libopenglwrapper/IOpenGLWrapperApp.hpp"

#include "SDL2Wrapper/WindowData.hpp"

using namespace LOGLW;

IOpenGLWrapperApp::IOpenGLWrapperApp( bool fullscreen, unsigned width, unsigned height, int x, int y, const char* winName, const char* configPath )
{
    SDL2W::WindowData windowData;
    windowData.name = "Test";
    windowData.pos = { x, y };
    windowData.currentRes.setSize( width, height );
    windowData.rendererName = "opengl";
    windowData.name = winName;

    m_sdlw = SDL2W::ISDL2Wrapper::createSDL2Wrapper();
    m_sdlw->init( windowData, configPath );
    m_sdlw->registerWindowEventListener( this );
    m_sdlw->registerKeyboardEventListener( this );
    m_sdlw->registerMouseEventListener( this );

    m_oglw = LOGLW::IOpenGLWrapper::createOpenGLWrapper( m_sdlw.get() );
    m_logger = m_oglw->getLoger();
    m_gutil = m_oglw->getUtility();

    m_oglw->onInitialize( [this](){ onInit(); } );
    m_oglw->beforeFrame( [this](){ customFrame(); } );
    m_sdlw->getMainWindow()->setFullscreen( fullscreen );

    LOGLW::ProjectionData g_projectionData;
    const auto& winSize = m_sdlw->getMainWindow()->getSize();
    g_projectionData.setSize( { winSize.getWidth(), winSize.getHeight() } );
    g_projectionData.setZnear( 127.0f );
    g_projectionData.setZfar( -64.0f );
    g_projectionData.setCenter( { 0.f, 0.f } );
    g_projectionData.setEyePos( { 0.f, 0.f, 128.f } );
    g_projectionData.m_projectionType = LOGLW::ProjectionType::PERSPECTIVE;
    m_oglw->setProjection( g_projectionData );

    m_objFactory = m_oglw->getObjectFactory();
}

IOpenGLWrapperApp::IOpenGLWrapperApp( bool fullscreen, unsigned width, unsigned height, WinPos pos, const char* winName, const char* configPath )
{
    SDL2W::WindowData windowData;
    windowData.name = "Test";
    windowData.currentRes.setSize( width, height );
    windowData.rendererName = "opengl";
    windowData.name = winName;

    m_sdlw = SDL2W::ISDL2Wrapper::createSDL2Wrapper();
    m_sdlw->init( windowData, configPath );
    m_sdlw->registerWindowEventListener( this );
    m_sdlw->registerKeyboardEventListener( this );
    m_sdlw->registerMouseEventListener( this );


    if( pos == LOGLW::IOpenGLWrapperApp::WinPos::CENTER )
    {
        auto screenSize = m_sdlw->getMainWindow()->getCurrentScreenNativeResolution();
        CUL::Graphics::Pos2Di newPos;
        newPos.setXY(
            ( screenSize.w - width ) / 2,
            ( screenSize.h - height ) / 2 );
        m_sdlw->getMainWindow()->setPos( newPos );
    }

    m_oglw = LOGLW::IOpenGLWrapper::createOpenGLWrapper( m_sdlw.get() );
    m_logger = m_oglw->getLoger();
    m_gutil = m_oglw->getUtility();

    m_oglw->onInitialize( [this](){ onInit(); } );
    m_oglw->beforeFrame( [this](){ customFrame(); } );
    m_sdlw->getMainWindow()->setFullscreen( fullscreen );

    LOGLW::ProjectionData g_projectionData;
    const auto& winSize = m_sdlw->getMainWindow()->getSize();
    g_projectionData.setSize( { winSize.getWidth(), winSize.getHeight() } );
    g_projectionData.setZnear( 127.0f );
    g_projectionData.setZfar( -64.0f );
    g_projectionData.setCenter( { 0.f, 0.f } );
    g_projectionData.setEyePos( { 0.f, 0.f, 128.f } );
    g_projectionData.m_projectionType = LOGLW::ProjectionType::PERSPECTIVE;
    m_oglw->setProjection( g_projectionData );

    m_objFactory = m_oglw->getObjectFactory();
}

void IOpenGLWrapperApp::run()
{
    m_oglw->startRenderingLoop();
    m_sdlw->runEventLoop();
}

void IOpenGLWrapperApp::onWindowEvent(const SDL2W::WindowEvent::Type)
{
}

void IOpenGLWrapperApp::onKeyBoardEvent(const SDL2W::IKey&)
{
}

void IOpenGLWrapperApp::onMouseEvent(const SDL2W::MouseData&)
{
}

void IOpenGLWrapperApp::close()
{
    m_oglw->stopRenderingLoop();
    m_sdlw->stopEventLoop();
}

IOpenGLWrapperApp::~IOpenGLWrapperApp()
{
}