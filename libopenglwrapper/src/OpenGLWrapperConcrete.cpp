#include "OpenGLWrapperConcrete.hpp"
#include "OpenGLShaderFactory.hpp"
#include "CUL/ConsoleUtilities.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "OpenGL_3_Utils.hpp"

using namespace LOGLW;

OpenGLWrapperConcrete::OpenGLWrapperConcrete(
    SDL2W::IWindow* window,
    SDL2W::ISDL2Wrapper* sdl2w ):
    m_sdlW( sdl2w ),
    m_activeWindow( window )
{
    CUL::Assert::simple( sdl2w, "NO SDL WRAPPER." );
    CUL::Assert::simple( window, "NO WINDOW." );
}

OpenGLWrapperConcrete::~OpenGLWrapperConcrete()
{
    if( m_oglContext )
    {
        SDL_GL_DeleteContext( m_oglContext );
        m_oglContext = nullptr; // This is basically void* !
    }
}

void OpenGLWrapperConcrete::startRenderingLoop()
{
    m_renderingLoopThread = std::thread( &OpenGLWrapperConcrete::renderLoop, this );
}

void OpenGLWrapperConcrete::stopRenderingLoop()
{
    m_runRenderLoop = false;
    if( m_renderingLoopThread.joinable() )
    {
        m_renderingLoopThread.join();
    }
}

void OpenGLWrapperConcrete::onInitialize( const std::function<void()>& callback )
{
    CUL::Assert::simple( m_hasBeenInitialized == false, "Wrapper already initialized, no need in defining " );
    m_onInitializeCallback = callback;
}

IShaderFactory* OpenGLWrapperConcrete::getShaderFactory()
{
    return &*m_shaderFactory;
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
    }
}

void OpenGLWrapperConcrete::initialize()
{
    m_oglContext = SDL_GL_CreateContext( m_activeWindow->getSDLWindow() );

    OGLUTILS::initContextVersion( 3, 1 );

    m_shaderFactory = new OpenGLShaderFactory();

    const auto& winSize = m_activeWindow->getSize();
    OGLUTILS::setViewport( 0, 0, winSize.getX(), winSize.getY() );
    OGLUTILS::setProjectionAndModelToIdentity();
    setBackgroundColor( ColorS( 0.0, 1.0, 0.0, 0.0 ) );

    if( m_onInitializeCallback )
    {
        m_onInitializeCallback();
    }

    m_hasBeenInitialized = true;
}


void OpenGLWrapperConcrete::executeTasks()
{
    while( m_preRenderTasks.size() > 0 )
    {
        auto task = m_preRenderTasks.front();
        task->execute();
        m_preRenderTasks.pop();
    }
}

void OpenGLWrapperConcrete::renderFrame()
{
    if( this->m_clearEveryFrame )
    {
        OGLUTILS::clearColorAndDepthBuffer();
    }
    setBackgroundColor( m_backgroundColor );

    renderObjects();
}

void OpenGLWrapperConcrete::renderObjects()
{
    for( auto& renderableObject : m_objectsToRender )
    {
        renderableObject->render();
    }
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

void OpenGLWrapperConcrete::test()
{

}