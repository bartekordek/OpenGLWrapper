#include "OpenGLWrapperConcrete.hpp"
#include "Primitives/TriangleImpl.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/ITimer.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/JSON/INode.hpp"
#include "Primitives/TriangleImpl.hpp"

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

IObject* OpenGLWrapperConcrete::createFromFile( const CsStr& path )
{
    const CUL::FS::Path filePath( path );
    if( ".json" == filePath.getExtension() )
    {
        const auto file = CUL::FS::FileFactory::createJSONFileRawPtr( path );
        file->load();
        return createFromFile( file );
    }
    return nullptr;
}

IObject* OpenGLWrapperConcrete::createFromFile( CUL::JSON::IJSONFile* file )
{
    auto root = file->getRoot();
    const auto type = root->getName();
    if( "default triangle" == type )
    {
        //TriangleData td;
        const auto vertices = root->getArray();
        for( const auto& vertex: vertices )
        {
            const auto typeVal = vertex->getType();
            std::cout << "WTF?" << (int) typeVal << "\n";
        }
        //const auto p1 = static_cast<const CUL::JSON::DataPair*>( vertices.at( 0 ) );
        //const auto p2 = static_cast<const CUL::JSON::DataPair*>( vertices.at( 1 ) );
        //const auto p3 = static_cast<const CUL::JSON::DataPair*>( vertices.at( 2 ) );

        //const auto p1Val = std::stof( p1->getVal() );
        //const auto p2Val = std::stof( p2->getVal() );
        //const auto p3Val = std::stof( p3->getVal() );

    }
    return nullptr;
}

IObject* OpenGLWrapperConcrete::createFromFile( IFile* file )
{
    const auto& fileExtension = file->getPath().getExtension();
    if( ".json" == fileExtension )
    {
        const auto jsonFile = static_cast<const CUL::JSON::IJSONFile*>( file );
        const auto root = jsonFile->getRoot();
        const auto name = root->findChild( "name" );
        const auto valueType = name->getType();
        CUL::Assert::simple( valueType == CUL::JSON::ElementType::STRING );

        auto nameVal = name->getString();
    }
    return nullptr;
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
    vp.setSize( winSize );
    vp.setEyePos( Pos3Df( 0.0f, 0.0f, 10.0f ) );
    vp.setCenter( Pos3Df( 0.0f, 0.0f, 0.0f ) );
    vp.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );

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
    OGLUTILS::resetMatrixToIdentity( GL_PROJECTION );
    if( ProjectionType::ORTO == type )
    {
        /*
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml
          Parameters
            left, right
            Specify the coordinates for the left and right vertical clipping planes.

            bottom, top
            Specify the coordinates for the bottom and top horizontal clipping planes.

            nearVal, farVal
            Specify the distances to the nearer and farther depth clipping planes.These values are negative if the plane is to be behind the viewer.*/
        const auto left = m_viewport.getLeft();
        const auto right = m_viewport.getRight();
        const auto bottom = m_viewport.getBottom();
        const auto top = m_viewport.getTop();
        const auto zNear = m_viewport.getZnear();
        const auto zFar = m_viewport.getZfar();

        glOrtho(
            left, // left
            right, // right
            bottom, // bottom
            top, // top
            zNear, // near
            zFar // far
        );
    }
    else if( ProjectionType::PERSPECTIVE == type )
    {
        gluPerspective( m_viewport.getFov(), m_viewport.getAspectRatio(), m_viewport.getZnear(), m_viewport.getZfar() );

        OGLUTILS::lookAt( m_viewport );
    }
    OGLUTILS::resetMatrixToIdentity( GL_MODELVIEW );
    m_currentProjection = type;
}

void OpenGLWrapperConcrete::setEyePos( const Pos3Df& pos )
{
    m_viewport.setEyePos( pos );
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
    for( auto& renderableObject : m_objectsToRender )
    {
        renderableObject->render();
    }
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