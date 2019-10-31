#include "OpenGLWrapperConcrete.hpp"
#include "OpenGLShaderFactory.hpp"
#include "Primitives/TriangleImpl.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/ITimer.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "OpenGL_3_Utils.hpp"

using namespace LOGLW;

OpenGLWrapperConcrete::OpenGLWrapperConcrete(
    SDL2W::IWindow* window,
    SDL2W::ISDL2Wrapper* sdl2w ):
    m_sdlW( sdl2w ),
    m_activeWindow( window ),
    m_logger( CUL::LOG::LOG_CONTAINER::getLogger() )
{
    CUL::Assert::simple( nullptr != sdl2w, "NO SDL WRAPPER." );
    CUL::Assert::simple( nullptr != window, "NO WINDOW." );
}

OpenGLWrapperConcrete::~OpenGLWrapperConcrete()
{
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete()..." );
    if( m_oglContext )
    {
        SDL_GL_DeleteContext( m_oglContext );
        m_oglContext = nullptr; // This is basically void* !
    }
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete() Done." );
}

void OpenGLWrapperConcrete::startRenderingLoop()
{
    m_logger->log( "OpenGLWrapperConcrete::startRenderingLoop()..." );
    m_renderingLoopThread = std::thread( &OpenGLWrapperConcrete::renderLoop, this );
    m_logger->log( "OpenGLWrapperConcrete::startRenderingLoop() Done." );
}

void OpenGLWrapperConcrete::stopRenderingLoop()
{
    m_logger->log( "OpenGLWrapperConcrete::stopRenderingLoop()..." );
    m_runRenderLoop = false;
    if( m_renderingLoopThread.joinable() )
    {
        m_renderingLoopThread.join();
    }
    m_logger->log( "OpenGLWrapperConcrete::stopRenderingLoop() Done." );
}

void OpenGLWrapperConcrete::onInitialize( const std::function<void()>& callback )
{
    CUL::Assert::simple( m_hasBeenInitialized == false, "Wrapper already initialized, no need in defining " );
    m_onInitializeCallback = callback;
}

void OpenGLWrapperConcrete::beforeFrame( const std::function<void()>& callback )
{
    m_onBeforeFrame = callback;
}

IShaderFactory* OpenGLWrapperConcrete::getShaderFactory()
{
    return &*m_shaderFactory;
}

IObjectFactory* OpenGLWrapperConcrete::getObjectFactory()
{
    return this;
}

CUL::LOG::ILogger* OpenGLWrapperConcrete::getLoger()
{
    return m_logger;
}

IRect* OpenGLWrapperConcrete::createRect()
{
    return nullptr;
}

ITriangle* OpenGLWrapperConcrete::createTriangle()
{
    auto result = new TriangleImpl();
    m_objectsToRender.insert( result );
    return result;
}

void OpenGLWrapperConcrete::renderLoop()
{
    initialize();

    CUL::ThreadUtils::setCurrentThreadName( "OpenGL render thread." );
    while( m_runRenderLoop )
    {
        executeTasks();
        renderFrame();
        refreshBuffers();
        CUL::ITimer::sleepMiliSeconds( 55 );
    }
}

void OpenGLWrapperConcrete::initialize()
{
    m_logger->log( "OpenGLWrapperConcrete::initialize()..." );
    m_oglContext = SDL_GL_CreateContext( m_activeWindow->getSDLWindow() );

    auto versionString = OGLUTILS::initContextVersion( 3, 1 );
    m_logger->log( "OpenGLWrapperConcrete::initialize(), OpenGL version:" );
    m_logger->log( versionString );

    m_shaderFactory = new OpenGLShaderFactory();

    const auto& winSize = m_activeWindow->getSize();

    OGLUTILS::setProjectionAndModelToIdentity();

    OGLUTILS::ViewPortRect rect;
    rect.pos.setXY( 0, 0 );
    rect.size.setWidth( winSize.getX() );
    rect.size.setHeight( winSize.getY() );
    OGLUTILS::setViewPort( rect );

    setBackgroundColor( ColorS( 0.0, 1.0, 0.0, 0.0 ) );

    if( m_onInitializeCallback )
    {
        m_onInitializeCallback();
    }

    OGLUTILS::listExtensions();

    m_hasBeenInitialized = true;
    m_logger->log( "OpenGLWrapperConcrete::initialize() Done." );
}

void OpenGLWrapperConcrete::executeTasks()
{
    while( false == m_preRenderTasks.empty() )
    {
        auto task = m_preRenderTasks.front();
        task->execute();
        m_preRenderTasks.pop();
    }
}

void OpenGLWrapperConcrete::renderFrame()
{
    setBackgroundColor( m_backgroundColor );
    if( m_clearEveryFrame )
    {
        OGLUTILS::clearColorAndDepthBuffer();
    }

    if( m_clearModelView )
    {
        OGLUTILS::resetMatrixToIdentity( GL_MODELVIEW );
    }

    if( m_onBeforeFrame )
    {
        m_onBeforeFrame();
    }
    renderObjects();
}

void OpenGLWrapperConcrete::renderObjects()
{
    for( auto& renderableObject: m_objectsToRender )
    {
        renderableObject->render();
    }
    glTranslatef( 2.0f, 2.0f, 0.0f );
    glScalef( 0.2f, 0.2f, 0.2f );
    OGLUTILS::createQuad();
}

void OpenGLWrapperConcrete::refreshBuffers()
{
    if( m_updateBuffers )
    {
        m_activeWindow->updateScreenBuffers();
    }
}

void OpenGLWrapperConcrete::setBackgroundColor(
    const ColorS& color )
{
    OGLUTILS::clearColorTo( color );
}