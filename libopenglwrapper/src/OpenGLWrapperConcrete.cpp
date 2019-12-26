#include "OpenGLWrapperConcrete.hpp"
#include "Primitives/TriangleImpl.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/ITimer.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "OpenGL_3_Utils.hpp"

using namespace LOGLW;

OpenGLWrapperConcrete::OpenGLWrapperConcrete(
    SDL2W::ISDL2Wrapper* sdl2w ):
    m_sdlW( sdl2w ),
    m_activeWindow( sdl2w->getMainWindow() ),
    m_logger( CUL::LOG::LOG_CONTAINER::getLogger() )
{
    CUL::Assert::simple( nullptr != sdl2w, "NO SDL WRAPPER." );
    CUL::Assert::simple( nullptr != m_activeWindow, "NO WINDOW." );
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

void OpenGLWrapperConcrete::onInitialize( const EmptyFunctionCallback& callback )
{
    CUL::Assert::simple( m_hasBeenInitialized == false, "Wrapper already initialized, no need in defining " );
    m_onInitializeCallback = callback;
}

void OpenGLWrapperConcrete::beforeFrame( const EmptyFunctionCallback& callback )
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

IProgramFactory* OpenGLWrapperConcrete::getProgramFactory()
{
    return &*m_shaderFactory;
}

IImageLoader* OpenGLWrapperConcrete::getImageLoader()
{
    return m_imageLoader;
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
    CUL::ThreadUtils::setCurrentThreadName( "OpenGL render thread." );

    initialize();
    while( m_runRenderLoop )
    {
        renderFrame();
    }
    release();
}

void OpenGLWrapperConcrete::initialize()
{
    m_logger->log( "OpenGLWrapperConcrete::initialize()..." );
    m_oglContext = SDL_GL_CreateContext( *m_activeWindow );

    auto versionString = OGLUTILS::initContextVersion( 3, 1 );
    m_logger->log( "OpenGLWrapperConcrete::initialize(), OpenGL version:" );
    m_logger->log( versionString );

    m_shaderFactory = new OpenGLShaderFactory();

    const auto& winSize = m_activeWindow->getSize();

    OGLUTILS::setProjectionAndModelToIdentity();

    Viewport vp;
    vp.size = winSize;
    setViewPort( vp );

    setBackgroundColor( ColorS( 0.0, 1.0, 0.0, 0.0 ) );

    auto extensionList = OGLUTILS::listExtensions();
    for( const auto& extension : extensionList )
    {
        std::cout << extension << "\n";
    }

    m_imageLoader = IImageLoader::createConcrete();

    m_hasBeenInitialized = true;
    m_logger->log( "OpenGLWrapperConcrete::initialize() Done." );

    if( m_onInitializeCallback )
    {
        m_onInitializeCallback();
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
        //OGLUTILS::resetMatrixToIdentity( GL_MODELVIEW );
        setProjectionType( m_currentProjection );
    }

    if( m_onBeforeFrame )
    {
        m_onBeforeFrame();
    }

    executeTasks();
    renderObjects();

    refreshBuffers();
    CUL::ITimer::sleepMicroSeconds( m_renderLoopLatencyUs );
}

void OpenGLWrapperConcrete::setProjectionType( const ProjectionType type )
{
    if( ProjectionType::ORTO == type )
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        /*
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml
          Parameters
            left, right
            Specify the coordinates for the left and right vertical clipping planes.

            bottom, top
            Specify the coordinates for the bottom and top horizontal clipping planes.

            nearVal, farVal
            Specify the distances to the nearer and farther depth clipping planes.These values are negative if the plane is to be behind the viewer.*/

        glOrtho(
            m_viewport.left, // left
            static_cast<double>( m_viewport.size.getWidth() ), // right
            static_cast<double>( m_viewport.size.getHeight() ), // bottom
            m_viewport.top, // top
            m_viewport.zNear, // near
            m_viewport.zFar // far
        );

        glMatrixMode( GL_MODELVIEW ); //Initialize Modelview Matrix
        glLoadIdentity();
    }
    else if( ProjectionType::PERSPECTIVE == type )
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( m_viewport.fov, m_viewport.getAspectRatio(), 1, 100 );

        OGLUTILS::lookAt( m_lookAt );
    }
    m_currentProjection = type;
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

void OpenGLWrapperConcrete::renderObjects()
{
    for( auto& renderableObject: m_objectsToRender )
    {
        renderableObject->render();
    }
    //glTranslatef( 2.0f, 2.0f, 0.0f );
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

void OpenGLWrapperConcrete::setRenderLoopLatency( Cunt uS )
{
    m_renderLoopLatencyUs = uS;
}

void OpenGLWrapperConcrete::setViewPort( const Viewport& rect )
{
    OGLUTILS::setViewPort( rect );
    m_viewport = rect;
}

void OpenGLWrapperConcrete::setBackgroundColor( const ColorS& color )
{
    OGLUTILS::clearColorTo( color );
}

OpenGLWrapperConcrete::~OpenGLWrapperConcrete()
{
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete()..." );
    release();
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete() Done." );
}

void OpenGLWrapperConcrete::release()
{
    if( m_oglContext )
    {
        m_shaderFactory.release();
        SDL_GL_DeleteContext( m_oglContext );
        m_oglContext = nullptr; // This is basically void* !
    }
}