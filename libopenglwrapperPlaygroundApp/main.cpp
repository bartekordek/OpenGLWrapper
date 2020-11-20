#include "MatrixStack.hpp"

#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "libopenglwrapper/IDebugOverlay.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/Math/Axis.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Math/Primitives/Triangle.hpp"
#include "CUL/STL_IMPORTS/STD_iostream.hpp" 
#include "CUL/Log/ILogger.hpp"
#include "CUL/GenericUtils/IConfigFile.hpp"

using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;
using WinEventType = SDL2W::WindowEvent::Type;
using ShaderFile = CUL::FS::IFile;
template <typename TYPE> using DumbPtr = CUL::GUTILS::DumbPtr<TYPE>;
using FF = CUL::FS::FileFactory;
using Rect = CUL::Graphics::Rect3Di;
using Cfloat = CUL::Cfloat;
using Pos3Df = CUL::Graphics::Pos3Df;
using Triangle = CUL::MATH::Primitives::TriangleF;
using String = CUL::String;

DumbPtr<CUL::GUTILS::IConfigFile> g_configFile;
DumbPtr<SDL2W::ISDL2Wrapper> g_sdlw;
GLWrap g_oglw;
LOGLW::IUtility* g_utility = nullptr;
CUL::LOG::ILogger* g_logger = nullptr;
LOGLW::MatrixStack matrixStack;
ColorS red( ColorE::RED );
ColorS yellow( 1.0f, 1.0f, 0.0f, 1.0f );
ColorS blue( ColorE::BLUE );
ColorS white( ColorE::WHITE );
GLfloat angle = 0.0f;
DumbPtr<LOGLW::Triangle> triangle;
LOGLW::IObjectFactory* of = nullptr;
CUL::FS::Path vertexShaderFile;
CUL::FS::Path fragmentShaderFile;
LOGLW::IProgram* program = nullptr;
SDL2W::WindowData windowData;
float blueTriangleZ = -1.0f;
float redTriangleZ = 1.0f;
LOGLW::ProjectionData g_projectionData;
Pos3Df g_eyePos;
LOGLW::IObject* g_triangle0 = nullptr;
CUL::TimeConcrete configModificationTime;
SDL2W::IWindow* g_mainWindow = nullptr;

Triangle triangleRed;
Triangle triangleYellow;
Triangle triangleBackground0;
Triangle triangleBackground1;

const CUL::String wrapperDir = "../libopenglwrapper";
const CUL::FS::Path shadersDir( wrapperDir + "/shaders/" );

float g_eyeZ = 20.f;
int g_mouseX = 0.0f;

void afterInit();
void renderScene();
void onKeyBoardEvent( const SDL2W::IKey& key );
void onWindowEvent( const WinEventType type );
void closeApp();
void reloadConfig();

void onMouseEvent( const SDL2W::IMouseData& md );

int main( int argc, char** argv )
{
    auto& argsInstance = CUL::GUTILS::ConsoleUtilities::getInstance();
    argsInstance.setArgs( argc, argv );

    windowData.name = "Test";
    windowData.pos = SDL2W::Vector3Di( 256, 256, 0 );
    auto posS = windowData.pos.serialize();

    windowData.size.setSize( 800, 600 );
    windowData.rendererName = "opengl";

    g_sdlw = SDL2W::createSDL2Wrapper();
    g_sdlw->init( windowData, "../media/Config.txt" );

    g_sdlw->addMouseEventCallback( onMouseEvent );

    g_configFile = g_sdlw->getConfig();

    g_oglw = LOGLW::createOpenGLWrapper( g_sdlw );
    g_logger = g_oglw->getLoger();
    g_logger->log( "\n" + posS );

    g_utility = g_oglw->getUtility();
    g_oglw->onInitialize( afterInit );
    g_oglw->beforeFrame( renderScene );

    g_sdlw->registerKeyboardEventCallback( &onKeyBoardEvent );
    g_sdlw->registerWindowEventCallback( &onWindowEvent );

    g_oglw->startRenderingLoop();

    g_sdlw->runEventLoop();

    return 0;
}

void afterInit()
{
    g_oglw->setProjectionType( LOGLW::ProjectionType::PERSPECTIVE );

    g_mainWindow = g_sdlw->getMainWindow();
    g_mainWindow->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ) );
    const auto& winSize = g_mainWindow->getSize();

    g_projectionData.setSize( {
        winSize.getWidth(),
        winSize.getHeight() } );
    const auto eyeZ = g_configFile->getValue( "EYE_Z" ).toFloat();
    g_projectionData.setEyePos( { 0.0f, 0.0f, eyeZ } );
    g_eyeZ = eyeZ;

    reloadConfig();
    configModificationTime = g_configFile->getModificationTime();

    g_mainWindow->toggleFpsCounter( true, 8u );

    g_oglw->drawDebugInfo( true );

    //g_oglw->getDebugOverlay()->addText( "Blue Z: %f", &blueTriangleZ );
    g_oglw->getDebugOverlay()->addSliderValue( "Blue Z", &blueTriangleZ, -64.0f, 128.f, [] ()
    {
    } );

    g_oglw->getDebugOverlay()->addSliderValue( "Red Z", &redTriangleZ, -64.0f, 128.f, [] ()
    {
    } );
}

void onMouseEvent( const SDL2W::IMouseData& )
{
    //auto newX = md.getX();
    //if( newX != (int) g_eyePos.z )
    //{
    //    auto diff = g_mouseX - newX;
    //    g_mouseX = newX;

    //    g_eyePos.z += (float)diff / 8.0f;
    //    g_logger->log( "New mouse x = " + String( g_eyePos.z ) );
    //    g_projectionData.setEyePos( g_eyePos );
    //    g_oglw->setProjection( g_projectionData );
    //}
}

void renderScene()
{
    matrixStack.push();
        g_utility->translate( 0.0f, 0.0f, blueTriangleZ );
        g_utility->draw( triangleBackground0, blue );
        g_utility->rotate( 180.0f, 0.0f, 0.0f, 1.0f );
        g_utility->draw( triangleBackground0, white );
    matrixStack.pop();

    matrixStack.push();
        g_utility->translate( 0.0f, 0.0f, redTriangleZ );
        g_utility->rotate( angle, 0.0f, 0.0f, 1.0f );
        g_utility->draw( triangleRed, red );
        g_utility->rotate( 180.0f, 0.0f, 0.0f, 1.0f );
        g_utility->draw( triangleRed, yellow );
    matrixStack.pop();

    angle += 0.8f;

    auto newTime = g_configFile->getModificationTime();
    if( newTime > configModificationTime )
    {
        g_logger->log( "Reloading..." );
        reloadConfig();
        g_logger->log( "Reloading... done." );

        configModificationTime = g_configFile->getModificationTime();
    }
}

void reloadConfig()
{
    g_configFile->reload();

    const auto x = 0.0f;
    const auto y = 0.0f;

    g_projectionData.setCenter( Pos3Df(
        x,
        g_configFile->getValue( "CENTER_Y" ).toFloat(),
        g_configFile->getValue( "CENTER_Z" ).toFloat() ) );
    g_eyePos = g_projectionData.getEye();
    g_eyePos.z = g_configFile->getValue( "EYE_Z" ).toFloat();
    g_projectionData.setEyePos( g_eyePos );

    g_projectionData.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );
    g_projectionData.setZfar( g_configFile->getValue( "Z_FAR" ).toFloat() );
    g_projectionData.setZnear( g_configFile->getValue( "Z_NEAR" ).toFloat() );
    g_oglw->setProjection( g_projectionData );

    float size = g_configFile->getValue( "RED_SIZE" ).toFloat();
    triangleRed.p1 = {  size, -size, 0.0f };
    triangleRed.p2 = { -size, -size, 0.0f };
    triangleRed.p3 = { -size,  size, 0.0f };

    size = g_configFile->getValue( "BLUE_SIZE" ).toFloat();
    triangleBackground0.p1 = {  size, -size, 0.0f };
    triangleBackground0.p2 = { -size, -size, 0.0f };
    triangleBackground0.p3 = { -size,  size, 0.0f };

    blueTriangleZ = g_configFile->getValue( "BLUE_Z" ).toFloat();
    redTriangleZ = g_configFile->getValue( "RED_Z" ).toFloat();
}

void onKeyBoardEvent( const SDL2W::IKey& key )
{
    if( key.getKeyIsDown() == false )
    {
        return;
    }

    const auto deltaZ = 1.0f;
    const auto delta = 2.0f;
    const CUL::String& keyName = key.getKeyName();
    if( keyName == "Q" )
    {
        closeApp();
    }
    else if( keyName == "W" )
    {
        blueTriangleZ -= deltaZ;
        g_logger->log( "setting blueTriangleZ to: " + String( blueTriangleZ ) );
    }
    else if( keyName == "S" )
    {
        blueTriangleZ += deltaZ;
        g_logger->log( "setting blueTriangleZ to: " + String( blueTriangleZ ) );
    }
    else if( keyName == "U" )
    {
        auto newVal = g_projectionData.getZfar() + delta;
        g_projectionData.setZfar( newVal );
        g_oglw->setProjection( g_projectionData );
        g_logger->log( "setting zFar to: " + String( newVal ) );
    }
    else if( keyName == "J" )
    {
        auto newVal = g_projectionData.getZfar() - delta;
        g_projectionData.setZfar( newVal );
        g_oglw->setProjection( g_projectionData );
        g_logger->log( "setting zFar to: " + String( newVal ) );
    }
    else if( keyName == "I" )
    {
        g_eyePos.z += 2.0f;
        g_oglw->setEyePos( g_eyePos );
        g_logger->log( "setting g_eyePos.z to: " + String( g_eyePos.z ) );
    }
    else if( keyName == "K" )
    {
        g_eyePos.z -= 2.0f;
        g_oglw->setEyePos( g_eyePos );
        g_logger->log( "setting g_eyePos.z to: " + String( g_eyePos.z ) );
    }
    else if( keyName == "T" )
    {
        redTriangleZ -= deltaZ;
        g_logger->log( "setting redTriangleZ to: " + String( redTriangleZ ) );
    }
    else if( keyName == "G" )
    {
        redTriangleZ += deltaZ;
        g_logger->log( "setting redTriangleZ to: " + String( redTriangleZ ) );
    }
    else if( keyName == "P" )
    {
        static bool toggle = true;
        if( toggle == true )
        {
            g_logger->log( "Changing projection to ortographic." );
            g_oglw->setProjectionType( LOGLW::ProjectionType::ORTO );
        }
        else
        {
            g_logger->log( "Changing projection to PERSPECTIVE." );
            g_oglw->setProjectionType( LOGLW::ProjectionType::PERSPECTIVE );
        }
        toggle = !toggle;
    }
}

void onWindowEvent( const WinEventType type )
{
    if( WinEventType::CLOSE == type )
    {
        closeApp();
    }
}

void closeApp()
{
    g_oglw->stopRenderingLoop();
    g_sdlw->stopEventLoop();
}