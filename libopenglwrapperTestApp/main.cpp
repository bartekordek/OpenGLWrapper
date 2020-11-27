#include "libopenglwrapper/IOpenGLWrapper.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/STL_IMPORTS/STD_memory.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"

using WinEventType = SDL2W::WindowEvent::Type;
using ShaderFile = CUL::GUTILS::DumbPtr<CUL::FS::IFile>;
using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using FF = CUL::FS::FileFactory;
using Pos3Df = CUL::Graphics::Pos3Df;

void onKeyBoardEvent( const SDL2W::IKey& key );
void onWindowEvent( const WinEventType type );

void closeApp();

GLWrap g_oglw;
SDL2W::ISDL2Wrapper* g_sdlw = nullptr;
LOGLW::IProgram* program = nullptr;
LOGLW::IObjectFactory* of = nullptr;

CUL::FS::Path vertexShaderFile;
CUL::FS::Path fragmentShaderFile;
LOGLW::ProjectionData projectionData;
Pos3Df eyePos;

void afterInit();

int main( int argc, char** argv )
{
    auto& argsInstance = CUL::GUTILS::ConsoleUtilities::getInstance();
    argsInstance.setArgs( argc, argv );

    SDL2W::WindowData windowData;
    windowData.name = "Test";
    windowData.pos = SDL2W::Vector3Di( 256, 256, 0 );
    windowData.size.setSize( 640, 480 );
    windowData.rendererName = "opengl";

    SDLWrap sdlW = SDL2W::createSDL2Wrapper();
    sdlW->init( windowData );
    g_sdlw = sdlW.get();
    auto window = sdlW->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorE::BLACK );
    window->toggleFpsCounter( true, 8u );

    g_oglw = LOGLW::createOpenGLWrapper( sdlW );
    g_oglw->onInitialize( afterInit );

    sdlW->registerKeyboardEventCallback( &onKeyBoardEvent );
    sdlW->registerWindowEventCallback( &onWindowEvent );

    g_oglw->startRenderingLoop();

    sdlW->runEventLoop();
}

void afterInit()
{
    auto sf = g_oglw->getShaderFactory();
    auto pf = g_oglw->getProgramFactory();

    const CUL::String wrapperDir = "../libopenglwrapper";
    const CUL::FS::Path shadersDir( wrapperDir + "/shaders/" );
    vertexShaderFile = shadersDir + "vertexShader.vert";
    fragmentShaderFile = shadersDir + "fragmentShader.frag";


    auto vs = sf->createShader( vertexShaderFile );
    auto fs = sf->createShader( fragmentShaderFile );

    program = pf->createProgram();

    program->attachShader( vs );
    program->attachShader( fs );
    program->link();
    program->validate();
    g_oglw->setProjectionType( LOGLW::ProjectionType::PERSPECTIVE );

    auto window = g_sdlw->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 0.0f, 0.0f, 0.0f, 1.0f ) );
    const auto& winSize = window->getSize();
    projectionData.setEyePos( { 0.f, 0.f, 170.f } );
    projectionData.setCenter( { 0.f, 0.f, 0.f } );
    projectionData.setUp( { 0.f, 1.f, 0.f } );
    g_oglw->setProjection( projectionData );

    of = g_oglw->getObjectFactory();


    const CUL::FS::Path defDir( wrapperDir + "/basic_definitions/" );

    of->createFromFile( defDir + "default_triangle.json" );
}

void onKeyBoardEvent( const SDL2W::IKey& key )
{
    if( key.getKeyName() == "q" || key.getKeyName() == "Q" )
    {
        closeApp();
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