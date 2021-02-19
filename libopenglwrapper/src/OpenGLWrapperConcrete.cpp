#include "OpenGLWrapperConcrete.hpp"

#include "TextureConcrete.hpp"
#include "Sprite.hpp"
#include "UtilConcrete.hpp"
#include "ImportImgui.hpp"

#include "Primitives/TriangleImpl.hpp"

#include "SDL2Wrapper/IWindow.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/ITimer.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/JSON/INode.hpp"
#include "CUL/STL_IMPORTS/STD_iostream.hpp"

using namespace LOGLW;

OpenGLWrapperConcrete::OpenGLWrapperConcrete(
    SDL2W::ISDL2Wrapper* sdl2w ):
    m_sdlW( sdl2w ),
    m_activeWindow( sdl2w->getMainWindow() ),
    m_cul( sdl2w->getCul() ),
    m_logger( sdl2w->getCul()->getLogger() ),
    m_oglUtility( new UtilConcrete( sdl2w->getCul() ) ),
    m_frameTimer( CUL::TimerFactory::getChronoTimer() )
{
    CUL::Assert::simple( nullptr != sdl2w, "NO SDL WRAPPER." );
    CUL::Assert::simple( nullptr != m_activeWindow, "NO WINDOW." );
    CUL::Assert::simple( nullptr != m_logger, "NO LOGGER." );

    registerObjectForUtility();
}

void OpenGLWrapperConcrete::registerObjectForUtility()
{
    TriangleImpl::useUtility( m_oglUtility );
    OpenGLShaderFactory::useUtility( m_oglUtility );
    Sprite::useUtility( m_oglUtility );
}

void OpenGLWrapperConcrete::startRenderingLoop()
{
    m_logger->log( "OpenGLWrapperConcrete::startRenderingLoop()..." );
    m_renderingLoopThread = std::thread( &OpenGLWrapperConcrete::mainThread, this );
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
    return m_shaderFactory.get();
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

const Viewport& OpenGLWrapperConcrete::getViewport() const
{
    return m_viewport;
}

ProjectionData& OpenGLWrapperConcrete::getProjectionData()
{
    return m_projectionData;
}

const ContextInfo& OpenGLWrapperConcrete::getContext() const
{
    return m_glContext;
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

    auto triangle = new TriangleImpl();

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

ITriangle* OpenGLWrapperConcrete::createTriangle( const ValuesArray& data, const ColorS& color )
{
    ITriangle* triangle = new TriangleImpl();
    triangle->setValues( data );
    triangle->setColor( color );
    addObjectToRender( triangle );
    return triangle;
}

ISprite* OpenGLWrapperConcrete::createSprite( const String& path )
{
    auto sprite = new Sprite();

    sprite->m_image = m_imageLoader->loadImage( path );
    sprite->m_textureId = m_oglUtility->generateTexture();
    m_oglUtility->bindTexture( sprite->m_textureId );

    const auto& ii = sprite->m_image->getImageInfo();

    TextureInfo td;
    td.pixelFormat = CUL::Graphics::PixelFormat::RGBA;
    td.size = ii.size;
    td.data = sprite->m_image->getData();
    m_oglUtility->setTextureData( td );

    m_oglUtility->setTextureParameter( TextureParameters::MAG_FILTER, TextureFilterType::LINEAR );
    m_oglUtility->setTextureParameter( TextureParameters::MIN_FILTER, TextureFilterType::LINEAR );

    m_oglUtility->bindTexture( 0 );

    addObjectToRender( sprite );

    return sprite;
}

void OpenGLWrapperConcrete::mainThread()
{
    CUL::ThreadUtils::setCurrentThreadName( "OpenGL render thread." );

    initialize();

    renderLoop();

    if( m_debugDrawInitialized )
    {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    release();
}

void OpenGLWrapperConcrete::renderLoop()
{
    while( m_runRenderLoop )
    {
        m_frameTimer->start();

        if( m_enableDebugDraw && !m_debugDrawInitialized )
        {
            initDebugInfo();
        }

        renderFrame();

        CUL::ITimer::sleepMicroSeconds( m_frameSleepUs );
        m_frameTimer->stop();

        m_currentFrameLengthUs = (int) m_frameTimer->getElapsed().getUs();

        calculateNextFrameLengths();
    }
}

void OpenGLWrapperConcrete::initDebugInfo()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL( *m_activeWindow, getContext().glContext );
    ImGui_ImplOpenGL2_Init();

    m_debugDrawInitialized = true;
}

void OpenGLWrapperConcrete::initialize()
{
    m_logger->log( "OpenGLWrapperConcrete::initialize()..." );

    m_glContext = m_oglUtility->initContextVersion( m_activeWindow, 4, 3 );
    //m_glContext = m_oglUtility->initContextVersion( m_activeWindow, 3, 1 );
    m_logger->log( "OpenGLWrapperConcrete::initialize(), OpenGL version:" );
    m_logger->log( m_glContext.glVersion );

    m_shaderFactory = new OpenGLShaderFactory( this );

    m_sdlW->registerSDLEventObserver( this );

    m_oglUtility->setProjectionAndModelToIdentity();
    m_oglUtility->setTexuring( true );

    const auto& winSize = m_activeWindow->getSize();
    setupProjectionData( winSize );

    m_viewport.pos.setXY( 0, 0 );
    m_viewport.size.setSize( winSize.getWidth(), winSize.getHeight() );

    m_logger->log( "Current viewport:" );
    m_logger->log( "\n" + m_viewport.serialize( 0 ) );

    m_backgroundColor.setAlphaF( 0.0 );
    setBackgroundColor( m_backgroundColor );

    showExtensions();

    m_imageLoader = IImageLoader::createConcrete( nullptr );

    calculateFrameWait();

    m_projectionData.m_depthTest.setOnChange( [this](){
        m_projectionChanged = true;
    } );

    m_projectionData.setOnChange( [this] () {
        m_projectionChanged = true;
    } );

    m_isPerspective.setOnChange( [this] ()
    {
        if( m_isPerspective )
        {
            m_projectionData.m_projectionType = ProjectionType::PERSPECTIVE;
        }
        else
        {
            m_projectionData.m_projectionType = ProjectionType::ORTO;
        }

        m_projectionChanged = true;
    } );

    //m_oglUtility->setBackfaceCUll(  );
    //m_oglUtility->setDepthTest( true );

    if( m_onInitializeCallback )
    {
        m_onInitializeCallback();
    }

    m_hasBeenInitialized = true;
    m_logger->log( "OpenGLWrapperConcrete::initialize() Done." );
}

void OpenGLWrapperConcrete::showExtensions()
{
    auto extensionList = m_oglUtility->listExtensions();
    for( const auto& extension : extensionList )
    {
        m_logger->log( "Extension: " + extension );
    }

    m_logger->log( "Extension count: " + std::to_string( extensionList.size() ) );
}

void OpenGLWrapperConcrete::setupProjectionData( const SDL2W::WindowSize& winSize )
{
    ProjectionData projectionData;
    projectionData.setSize( winSize );
    projectionData.setEyePos( Pos3Df( 0.0f, 0.0f, 220.0f ) );
    projectionData.setCenter( Pos3Df(
        static_cast<float>(winSize.getWidth()) / 2.f,
        static_cast<float>(winSize.getHeight()) / 2.f, 0.0f ) );
    projectionData.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );
    projectionData.m_zFar = -64.f;
    projectionData.m_zNear = 64.f;
    setProjection( projectionData );
}

void OpenGLWrapperConcrete::renderFrame()
{
    //setBackgroundColor( m_backgroundColor );
    if( m_clearEveryFrame )
    {
        m_oglUtility->clearColorAndDepthBuffer();
    }

    m_oglUtility->setDepthTest( m_projectionData.m_depthTest );

    m_projectionChanged = true;
    if( m_projectionChanged )
    {
        changeProjectionType();
        m_projectionChanged = false;
    }



    if( m_clearModelView )
    {
        //m_oglUtility->resetMatrixToIdentity( MatrixTypes::MODELVIEW );
    }


    if( m_onBeforeFrame )
    {
        m_onBeforeFrame();
    }


    //m_oglUtility->setDepthTest( true );
   // m_oglUtility->setBackfaceCUll( true );

    if( m_viewportChanged )
    {
        m_oglUtility->setViewport( m_viewport );
        m_viewportChanged = false;
    }

    executeTasks();
    renderObjects();

    if( m_enableDebugDraw )
    {
        renderInfo();
    }

    refreshBuffers();

}

void OpenGLWrapperConcrete::calculateNextFrameLengths()
{
    if( m_currentFrameLengthUs > m_targetFrameLengthUs + m_usRes )
    {
        m_frameSleepUs -= m_usDelta;
        if( m_frameSleepUs < 0 )
        {
            m_frameSleepUs = 0;
        }
    }
    else if( m_currentFrameLengthUs < m_targetFrameLengthUs - m_usRes )
    {
        m_frameSleepUs += m_usDelta;
    }
    m_usDelta = std::abs( m_currentFrameLengthUs - m_targetFrameLengthUs ) / 4;
}

#if _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4061)
#endif
void OpenGLWrapperConcrete::renderInfo()
{
    ImGui::SetNextWindowPos( { 0, 0 } );
    const auto& winSize = m_activeWindow->getSize();

    ImGui_ImplOpenGL2_NewFrame();

    ImGui_ImplSDL2_NewFrame( *m_activeWindow );

    ImGui::NewFrame();

    String name = "INFO LOG";
    ImGui::Begin( name.cStr() );
    ImGui::SetWindowSize( { (float) winSize.getWidth() * 0.3f, (float) winSize.getHeight() * 1.f } );

    auto res = false;
    ImGui::Checkbox( "Projection is Perspective", &m_isPerspective.getRef() );
    m_isPerspective.runIfChanged();

    ImGui::Checkbox( "Depth test", &m_projectionData.m_depthTest.getRef() );
    m_projectionData.m_depthTest.runIfChanged();

    ImGui::Text( "Projection: %s", ( m_projectionData.m_projectionType == ProjectionType::PERSPECTIVE)? "Perspective" : "Orthogonal" );
    ImGui::Text( "Aspect Ratio: %f", m_projectionData.getAspectRatio() );
    ImGui::Text( "FOV-Y: %f", m_projectionData.getFov() );

    String text = "Center:" + m_projectionData.getCenter().serialize( 0 );
    ImGui::Text( "%s",text.cStr() );

    text = "Eye:" + m_projectionData.getEye().serialize( 0 );
    ImGui::Text( "%s",text.cStr() );

    text = "Up:" + m_projectionData.getUp().serialize( 0 );
    ImGui::Text( "%s",text.cStr() );

    const auto& mData = m_sdlW->getMouseData();
    text = "Mouse = ( " + String( mData.getX() ) + ", " + String( mData.getY() ) + " )";
    ImGui::Text( "%s", text.cStr() );

    res = ImGui::SliderFloat( "Z Far", &m_projectionData.m_zFar, -64.f, 64.f );
    if( res )
    {
        m_projectionChanged = true;
    }

    res = ImGui::SliderFloat( "Z Near", &m_projectionData.m_zNear, -63.f, 255.f );
    if( res )
    {
        m_projectionChanged = true;
    }

    res = ImGui::SliderFloat( "Eye-Z", &m_projectionData.m_eye.z, 0.0f, 255.0f );
    if( res )
    {
        //m_projectionData.setZnear( m_projectionData.m_eye.z );
        m_projectionChanged = true;
    }

    res = ImGui::SliderFloat( "Center-Z", &m_projectionData.m_center.z, -64.0f, 255.0f );
    if( res )
    {
        m_projectionChanged = true;
    }

    text = "Left: " + String( m_projectionData.getLeft() );
    ImGui::Text( "%s",text.cStr() );

    text = "Right: " + String( m_projectionData.getRight() );
    ImGui::Text( "%s",text.cStr() );

    text = "Top: " + String( m_projectionData.getTop() );
    ImGui::Text( "%s",text.cStr() );

    text = "Bottom: " + String( m_projectionData.getBottom() );
    ImGui::Text( "%s",text.cStr() );

    for( const auto& pair: m_debugValues )
    {
        if( pair.second.type == DebugType::TEXT )
        {
            const size_t id = pair.second.value.index();
            switch( id )
            {
                case 0:
                    ImGui::Text( pair.second.text.cStr(), *(const char*)std::get<String*>( pair.second.value ) );
                    break;
                case 1:
                    ImGui::Text( pair.second.text.cStr(), *std::get<float*>( pair.second.value ) );
                    break;
                case 2:
                    ImGui::Text( pair.second.text.cStr(), *std::get<int*>( pair.second.value ) );
                    break;
                default:
                    break;
            }
        }
        else if( pair.second.type == DebugType::SLIDER )
        {
            const size_t id = pair.second.value.index();
            bool changed = false;
            switch( id )
            {
                case 0:
                    //ImGui::Text( pair.second.text.cStr(),  );
                    //const auto changed = ImGui::SliderFloat( pair.second.text.cStr(), *std::get<String*>( pair.second.value ) 0.0f, 192.0f );
                    break;
                case 1:
                    changed = ImGui::SliderFloat( pair.second.text.cStr(), std::get<float*>( pair.second.value ), pair.second.min, pair.second.max );
                    break;
                case 2:
                    changed = ImGui::SliderInt( pair.second.text.cStr(), std::get<int*>( pair.second.value ), (int)pair.second.min, (int)pair.second.max );
                    break;
                default:
                    break;
            }

            if( changed && pair.second.onChange )
            {
                pair.second.onChange();
            }
        }
    }

    ImGui::Text( "FrameTime: %4.2f ms", 1000.f / ImGui::GetIO().Framerate );
    ImGui::Text( "FPS: %4.2f", m_activeWindow->getFpsCounter()->getCurrentFps() );

    ImGui::Text( "m_frameSleepUs: %d", m_frameSleepUs.getValCopy() );
    ImGui::Text( "m_usDelta: %d", m_usDelta );

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL2_RenderDrawData( ImGui::GetDrawData() );
}
#if _MSC_VER
#pragma warning( pop )
#endif

void OpenGLWrapperConcrete::setProjectionType( const ProjectionType type )
{
    m_projectionData.m_projectionType = type;
    prepareProjection();
    m_projectionChanged = true;
}

void OpenGLWrapperConcrete::prepareProjection()
{
    //if( ProjectionType::ORTO == m_projectionData.m_projectionType )
    //{
    //    auto eyeCopy = m_projectionData.getEye();
    //    eyeCopy.z = std::max( eyeCopy.z, m_projectionData.getZnear() );
    //    m_projectionData.setZnear( eyeCopy.z );
    //    m_projectionData.setEyePos( eyeCopy );
    //}
    //else if( ProjectionType::PERSPECTIVE == m_projectionData.m_projectionType )
    //{
    //    m_projectionData.setZnear( std::min( m_projectionData.getEye().z, m_projectionData.getZnear() ) );
    //}
}

void OpenGLWrapperConcrete::changeProjectionType()
{
    m_oglUtility->resetMatrixToIdentity( MatrixTypes::PROJECTION );
    if( ProjectionType::ORTO == m_projectionData.m_projectionType )
    {

        m_oglUtility->setOrthogonalPerspective( m_projectionData );
    }
    else if( ProjectionType::PERSPECTIVE == m_projectionData.m_projectionType )
    {
        m_oglUtility->setPerspectiveProjection( m_projectionData );

    }
    m_oglUtility->resetMatrixToIdentity( MatrixTypes::MODELVIEW );
    m_oglUtility->lookAt( m_projectionData );
    m_currentProjection = m_projectionData.m_projectionType;
    setProjection( m_projectionData );
    m_oglUtility->setDepthTest( m_projectionData.m_depthTest );
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

void OpenGLWrapperConcrete::setProjection( const ProjectionData& rect )
{
    m_oglUtility->setProjection( rect );

    m_projectionData = rect;
    m_projectionChanged = true;
}

void OpenGLWrapperConcrete::setViewport( const Viewport& viewport, const bool instant )
{
    if( m_viewport != viewport )
    {
        m_viewport = viewport;
        if( false == instant )
        {
            m_viewportChanged = true;
        }
        else
        {
            m_oglUtility->setViewport( m_viewport );
        }
    }
}

void OpenGLWrapperConcrete::setBackgroundColor( const ColorS& color )
{
    m_oglUtility->clearColorTo( color );
    m_backgroundColor = color;
}

void OpenGLWrapperConcrete::release()
{
    m_logger->log( "OpenGLWrapperConcrete::release()..." );

    m_sdlW->unRegisterSDLEventObserver( this );

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

void OpenGLWrapperConcrete::calculateFrameWait()
{
    m_targetFrameLengthUs = 1000000.0f / m_fpsLimit;
}

CUL::GUTILS::IConfigFile* OpenGLWrapperConcrete::getConfig()
{
    CUL::Assert::simple( m_sdlW != nullptr, "No proper SDL2 pointer initialized." );

    return m_sdlW->getConfig();
}

void OpenGLWrapperConcrete::drawDebugInfo( const bool enable )
{
    m_enableDebugDraw = enable;
}

IDebugOverlay* OpenGLWrapperConcrete::getDebugOverlay()
{
    return this;
}

void OpenGLWrapperConcrete::handleEvent( const SDL_Event& event )
{
    if( m_debugDrawInitialized )
    {
        ImGui_ImplSDL2_ProcessEvent( &event );
    }
}

unsigned OpenGLWrapperConcrete::addSliderValue( const CUL::String& text, float* val, float min, float max, const std::function<void( void )> onUpdate )
{
    const unsigned size = (unsigned)m_debugValues.size();
    const auto newId = size + 1u;

    DebugValueRow row;
    row.type = DebugType::SLIDER;
    row.id = newId;
    row.min = min;
    row.max = max;
    row.value = val;
    row.text = text;
    row.onChange = onUpdate;

    m_debugValues[ newId ] = row;

    return newId;
}

unsigned OpenGLWrapperConcrete::addText( const CUL::String& text, float* val )
{
    const unsigned size = (unsigned) m_debugValues.size();
    const auto newId = size + 1u;

    DebugValueRow row;
    row.type = DebugType::TEXT;
    row.id = newId;
    row.value = val;
    row.text = text;

    m_debugValues[ newId ] = row;

    return newId;
}

void OpenGLWrapperConcrete::runEventLoop()
{
    m_sdlW->runEventLoop();
}

void OpenGLWrapperConcrete::stopEventLoop()
{
    m_sdlW->stopEventLoop();
}

SDL2W::IWindow* OpenGLWrapperConcrete::getMainWindow()
{
    return m_sdlW->getMainWindow();
}

ITextureFactory* OpenGLWrapperConcrete::getTextureFactory()
{
    return this;
}

ITexture* OpenGLWrapperConcrete::createTexture( const CUL::FS::Path& path, const bool )
{
    //auto image = m_sdlW->getCul()->getImageLoader()->loadImage( path, rgba );
    auto textureConcrete = new TextureConcrete( getUtility(), m_sdlW->getCul()->getImageLoader(), path );
    return textureConcrete;
}

// SDL2W::IMouseObservable
void OpenGLWrapperConcrete::addMouseEventCallback( const SDL2W::IMouseObservable::MouseCallback& callback )
{
    m_sdlW->addMouseEventCallback( callback );
}

void OpenGLWrapperConcrete::registerMouseEventListener( SDL2W::IMouseObserver* observer )
{
    m_sdlW->registerMouseEventListener( observer );
}

void OpenGLWrapperConcrete::unregisterMouseEventListener( SDL2W::IMouseObserver* observer )
{
    m_sdlW->unregisterMouseEventListener( observer );
}

SDL2W::MouseData& OpenGLWrapperConcrete::getMouseData()
{
    return m_sdlW->getMouseData();
}

// SDL2W::IKeyboardObservable
void OpenGLWrapperConcrete::registerKeyboardEventCallback( const std::function<void( const SDL2W::IKey& key )>& callback )
{
    m_sdlW->registerKeyboardEventCallback( callback );
}

void OpenGLWrapperConcrete::registerKeyboardEventListener( SDL2W::IKeyboardObserver* observer )
{
    m_sdlW->registerKeyboardEventListener( observer );
}

void OpenGLWrapperConcrete::unregisterKeyboardEventListener( SDL2W::IKeyboardObserver* observer )
{
    m_sdlW->unregisterKeyboardEventListener( observer );
}

bool OpenGLWrapperConcrete::isKeyUp( const String& keyName ) const
{
    return m_sdlW->isKeyUp( keyName );
}

void OpenGLWrapperConcrete::registerWindowEventCallback( const SDL2W::WindowCallback& callback )
{
    m_sdlW->registerWindowEventCallback( callback );
}

CUL::CULInterface* OpenGLWrapperConcrete::getCul()
{
    return m_sdlW->getCul();
}

OpenGLWrapperConcrete::~OpenGLWrapperConcrete()
{
    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete()..." );

    m_logger->log( "OpenGLWrapperConcrete::~OpenGLWrapperConcrete() Done." );
}