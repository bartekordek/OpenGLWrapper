#include "OpenGLWrapperConcrete.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/ITimer.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/JSON/INode.hpp"
#include "libopenglwrapper/Primitives/Triangle.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "UtilConcrete.hpp"

using namespace LOGLW;

OpenGLWrapperConcrete::OpenGLWrapperConcrete(
    SDL2W::ISDL2Wrapper* sdl2w ):
    m_sdlW( sdl2w ),
    m_activeWindow( sdl2w->getMainWindow() ),
    m_cul( sdl2w->getCul() ),
    m_logger( CUL::LOG::LOG_CONTAINER::getLogger() ),
    m_oglUtility( new UtilConcrete( sdl2w->getCul() ) )
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

void OpenGLWrapperConcrete::addObjectToRender( IRenderable* renderable )
{
    std::lock_guard<std::mutex> lockGuard( m_objectsToRenderMtx );
    m_objectsToRender.insert( renderable );
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

IUtility* OpenGLWrapperConcrete::getUtility()
{
    return m_oglUtility;
}

IRect* OpenGLWrapperConcrete::createRect()
{
    return nullptr;
}

Triangle* OpenGLWrapperConcrete::createTriangle()
{
    /*
    auto result = new TriangleImpl();
    m_objectsToRender.insert( result );
    */
    return nullptr;
}   

IObject* OpenGLWrapperConcrete::createFromFile( const String& path )
{
    const CUL::FS::Path filePath( path );
    if( ".json" == filePath.getExtension() )
    {
        const auto file = m_cul->getFF()->createJSONFileRawPtr( path );
        file->load();
        return createFromFile( file );
    }
    return nullptr;
}

IObject* OpenGLWrapperConcrete::createFromFile( CUL::JSON::IJSONFile* file )
{
    auto root = file->getRoot();
    const auto nameNode = root->findChild( "name" );
    CUL::Assert::simple( nameNode->getType() == CUL::JSON::ElementType::STRING, "Wrong JSON definition: type of name value." );
    if( "default triangle" == nameNode->getString() )
    {
        return createTriangle( root->findChild( "vertices" ) );
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

IObject* OpenGLWrapperConcrete::createTriangle( CUL::JSON::INode* jNode )
{
    CUL::Assert::simple( CUL::JSON::ElementType::ARRAY == jNode->getType(), "Different types." );
    CUL::Assert::simple( 3 == jNode->getArray().size(), "Defined triangle vertices count mismatch." );

    auto triangle = new Triangle();

    auto jsonToPoint = []( CUL::JSON::INode* node ) -> Point
    {
        CUL::Assert::simple( node->getType() == CUL::JSON::ElementType::ARRAY, "Vertice data type mismatch." );

        auto px = node->findChild( "x" );
        auto py = node->findChild( "y" );
        auto pz = node->findChild( "z" );

        Point point;
        point.x = px->getDouble();
        point.y = py->getDouble();
        point.z = pz->getDouble();
        return point;
    };

    const auto vertex1 = jNode->getArray()[0];
    const auto vertex2 = jNode->getArray()[1];
    const auto vertex3 = jNode->getArray()[2];

    triangle->setP1( jsonToPoint( vertex1 ) );
    triangle->setP2( jsonToPoint( vertex2 ) );
    triangle->setP3( jsonToPoint( vertex3 ) );

    return triangle;
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

    m_glContext = m_oglUtility->initContextVersion( m_activeWindow, 3, 1 );
    m_logger->log( "OpenGLWrapperConcrete::initialize(), OpenGL version:" );
    m_logger->log( m_glContext.glVersion );

    m_shaderFactory = new OpenGLShaderFactory( this );
    m_shaderFactory->useUtility( m_oglUtility );


    m_oglUtility->setProjectionAndModelToIdentity();

    const auto& winSize = m_activeWindow->getSize();
    ProjectionData projectionData;
    projectionData.setSize( winSize );
    projectionData.setEyePos( Pos3Df( 0.0f, 0.0f, 10.0f ) );
    projectionData.setCenter( Pos3Df(
        static_cast<float>( winSize.getWidth() ) / 2.f,
        static_cast<float>( winSize.getHeight() ) / 2.f, 0.0f ) );
    projectionData.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );

    setProjection( projectionData );

    m_viewport.pos.setXY( 0, 0 );
    m_viewport.size.setSize( winSize.getWidth(), winSize.getHeight() );

    m_backgroundColor.setAlphaF( 0.0 );
    setBackgroundColor( m_backgroundColor );

    auto extensionList = m_oglUtility->listExtensions();
    for( const auto& extension: extensionList )
    {
        m_logger->log( "Extension: " + extension );
    }

    m_logger->log( "Extension count: " + std::to_string( extensionList.size() ) );

    m_imageLoader = IImageLoader::createConcrete( nullptr );

    m_oglUtility->setDepthTest( true );

    if( m_onInitializeCallback )
    {
        m_onInitializeCallback();
    }

    m_hasBeenInitialized = true;
    m_logger->log( "OpenGLWrapperConcrete::initialize() Done." );
}

CUL::CULInterface* OpenGLWrapperConcrete::getCul()
{
    return m_sdlW->getCul();
}

void OpenGLWrapperConcrete::renderFrame()
{
    //setBackgroundColor( m_backgroundColor );
    if( m_clearEveryFrame )
    {
        m_oglUtility->clearColorAndDepthBuffer();
    }

    if( m_projectionChanged )
    {
        changeProjectionType();
        
        m_projectionChanged = false;
    }

    if( m_clearModelView )
    {
        
        m_oglUtility->resetMatrixToIdentity( MatrixTypes::MODELVIEW );
    }

    if( m_onBeforeFrame )
    {
        m_onBeforeFrame();
    }

    if( m_viewportChanged )
    {
        m_oglUtility->setViewport( m_viewport );
    }

    executeTasks();
    renderObjects();

    refreshBuffers();
    CUL::ITimer::sleepMicroSeconds( m_renderLoopLatencyUs );
}

void OpenGLWrapperConcrete::setProjectionType( const ProjectionType type )
{
    m_projectionType = type;
    m_projectionChanged = true;
}

void OpenGLWrapperConcrete::changeProjectionType()
{
    m_oglUtility->resetMatrixToIdentity( MatrixTypes::PROJECTION );
    if( ProjectionType::ORTO == m_projectionType )
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
        m_oglUtility->setOrthogonalPerspective( m_projectionData );
    }
    else if( ProjectionType::PERSPECTIVE == m_projectionType )
    {
        m_oglUtility->setPerspectiveProjection( m_projectionData );
        m_oglUtility->lookAt( m_projectionData );
    }
    m_currentProjection = m_projectionType;
    setProjection( m_projectionData );
}

void OpenGLWrapperConcrete::setEyePos( const Pos3Df& pos )
{
    m_projectionData.setEyePos( pos );
    m_projectionChanged = true;
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
    std::lock_guard<std::mutex> lockGuard( m_objectsToRenderMtx );
    for( auto& renderableObject : m_objectsToRender )
    {
        renderableObject->render();
    }
    if( m_drawQuad ) m_oglUtility->createQuad();
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

void OpenGLWrapperConcrete::setProjection( const ProjectionData& rect )
{
    m_oglUtility->setProjection( rect );

    m_projectionData = rect;
    m_projectionChanged = true;
}

void OpenGLWrapperConcrete::setViewport( const Viewport& viewport )
{
    m_viewport = viewport;
    m_viewportChanged = true;
}

void OpenGLWrapperConcrete::setBackgroundColor( const ColorS& color )
{
    m_oglUtility->clearColorTo( color );
    m_backgroundColor = color;
}

OpenGLWrapperConcrete::~OpenGLWrapperConcrete()
{
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete()..." );

    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete() Done." );
}

void OpenGLWrapperConcrete::release()
{
    m_logger->log( "OpenGLWrapperConcrete::release()..." );
    m_shaderFactory.release();

    m_oglUtility->destroyContext( m_glContext );

    delete m_oglUtility;
    m_oglUtility = nullptr;
    m_logger->log( "OpenGLWrapperConcrete::release()... Done." );
}

void OpenGLWrapperConcrete::drawQuad( const bool draw )
{
    m_drawQuad = draw;
}

void OpenGLWrapperConcrete::clearModelViewEveryFrame( const bool enable )
{
    m_clearModelView = enable;
}