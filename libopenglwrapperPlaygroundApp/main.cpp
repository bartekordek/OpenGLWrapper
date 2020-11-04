#include "MatrixStack.hpp"
#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "SDL2Wrapper/ISDL2Wrapper.hpp"
#include "CUL/Math/Axis.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Math/Primitives/Triangle.hpp"
#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "CUL/Log/ILogger.hpp"

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
float objectZ = 0.0f;
LOGLW::ProjectionData g_projectionData;
Pos3Df g_eyePos;
LOGLW::IObject* g_triangle0 = nullptr;

Triangle triangleRed;
Triangle triangleYellow;
Triangle triangleBackground0;
Triangle triangleBackground1;

const CUL::String wrapperDir = "../libopenglwrapper";
const CUL::FS::Path shadersDir( wrapperDir + "/shaders/" );

void afterInit();
void renderScene();
void onKeyBoardEvent( const SDL2W::IKey& key );
void onWindowEvent( const WinEventType type );
void closeApp();

int main( int argc, char** argv )
{
    auto& argsInstance = CUL::GUTILS::ConsoleUtilities::getInstance();
    argsInstance.setArgs( argc, argv );

    windowData.name = "Test";
    windowData.pos = SDL2W::Vector3Di( 256, 256, 0 );
    windowData.size.setSize( 1280, 800 );
    windowData.rendererName = "opengl";

    g_sdlw = SDL2W::createSDL2Wrapper();
    g_sdlw->init( windowData );

    g_oglw = LOGLW::createOpenGLWrapper( g_sdlw );
    g_logger = g_oglw->getLoger();

    g_utility = g_oglw->getUtility();
    g_oglw->onInitialize( afterInit );
    g_oglw->beforeFrame( renderScene );
    g_oglw->setRenderLoopLatency( 8192 );

    g_sdlw->registerKeyboardEventCallback( &onKeyBoardEvent );
    g_sdlw->registerWindowEventCallback( &onWindowEvent );

    g_oglw->startRenderingLoop();

    g_sdlw->runEventLoop();

    return 0;
}

void afterInit()
{
    auto sf = g_oglw->getShaderFactory();
    auto pf = g_oglw->getProgramFactory();


    vertexShaderFile = shadersDir + "simpleVertex.vert";
    fragmentShaderFile = shadersDir + "simpleFrag.frag";

    auto vs = sf->createShader( vertexShaderFile );
    auto fs = sf->createShader( fragmentShaderFile );

    program = pf->createProgram();

    program->attachShader( vs );
    program->attachShader( fs );
    program->link();
    program->validate();
    g_oglw->setProjectionType( LOGLW::ProjectionType::PERSPECTIVE );

    auto window = g_sdlw->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ) );
    const auto& winSize = window->getSize();
    g_projectionData.setSize( winSize );
    g_eyePos.z = 70.0f;
    g_projectionData.setEyePos( g_eyePos );
    g_projectionData.setCenter( Pos3Df( 0.0f, 0.0f, 0.0f ) );
    g_projectionData.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );
    g_projectionData.setZfar( -16.0f );
    g_projectionData.setZnear( 8.0f );
    g_oglw->setProjection( g_projectionData );

    of = g_oglw->getObjectFactory();

    const CUL::FS::Path defDir( wrapperDir + "/basic_definitions/" );

    g_triangle0 = of->createFromFile( defDir + "default_triangle.json" );

    g_triangle0->setPosition( { -64, 64, 0 } );

    window->toggleFpsCounter( true, 4u );
    const float size = 64.0f;

    triangleRed.p1 = {  size, -size, 0.0f };
    triangleRed.p2 = { -size, -size, 0.0f };
    triangleRed.p3 = { -size,  size, 0.0f };

}

void renderScene()
{
    matrixStack.push();
    g_utility->translate( 0.0f, 0.0f, objectZ );
    g_utility->draw( triangleRed, blue );


    matrixStack.pop();

    matrixStack.push();

    g_utility->rotate( angle, 0.0f, 0.0f, 1.0f );
    //glTranslatef( 0.0f, 32.0f, objectZ );

    //TODO:
//    g_triangle0->render();
    g_utility->draw( triangleRed, red );
    g_utility->rotate( 180.0f, 0.0f, 0.0f, 1.0f );
    g_utility->draw( triangleRed, yellow );
    matrixStack.pop();

    angle += 0.8f;
}

void onKeyBoardEvent( const SDL2W::IKey& key )
{
    if( key.getKeyIsDown() == false )
    {
        return;
    }

    const auto deltaZ = 2.0f;
    const auto delta = 4.0f;
    const CUL::String& keyName = key.getKeyName();
    if( keyName == "Q" )
    {
        closeApp();
    }
    else if( keyName == "W" )
    {
        objectZ -= deltaZ;
        g_logger->log( "setting objectZ to: " + String( objectZ ) );
    }
    else if( keyName == "S" )
    {
        objectZ += deltaZ;
        g_logger->log( "setting objectZ to: " + String( objectZ ) );
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