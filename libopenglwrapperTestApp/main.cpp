#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "libopenglwrapper/Primitives/Triangle.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"

#include "CUL/STL_IMPORTS/STD_memory.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

#if _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 )
#endif

using WinEventType = SDL2W::WindowEvent::Type;
using ShaderFile = CUL::GUTILS::DumbPtr<CUL::FS::IFile>;
using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using FF = CUL::FS::FileFactory;

void onKeyBoardEvent( const SDL2W::IKey& key );
void onWindowEvent( const WinEventType type);

void closeApp();

GLWrap g_oglw;
SDL2W::ISDL2Wrapper* g_sdlw = nullptr;

ShaderFile vertexShaderFile;
ShaderFile fragmentShaderFile;
LOGLW::Triangle triangle;

CUL::FS::IFile* getFile( const CUL::FS::Path& filePath );

void afterInit()
{
    auto sf = g_oglw->getShaderFactory();
    CUL::FS::Path shadersDir( "../libopenglwrapper/shaders/" );
    vertexShaderFile = FF::createRegularFileRawPtr( shadersDir + "vertexShader.vert" );
    fragmentShaderFile = FF::createRegularFileRawPtr( shadersDir + "fragmentShader.frag" );

    vertexShaderFile->load( true );
    fragmentShaderFile->load( true );


}

int main( int argc, char** argv )
{
    SDLWrap sdlW = SDL2W::createSDL2Wrapper(
        SDL2W::Vector3Di( 256, 256, 0 ),
        SDL2W::Vector3Du( 640, 480, 0 ), "Test", true );
    g_sdlw = sdlW.get();
    auto window = sdlW->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ));

    g_oglw = LOGLW::createOpenGLWrapper( window, sdlW.get() );
    g_oglw->onInitialize( afterInit );

    sdlW->addKeyboardEventCallback( &onKeyBoardEvent );
    sdlW->registerWindowEventCallback( &onWindowEvent );

    g_oglw->startRenderingLoop();

    sdlW->runEventLoop();
}

CUL::FS::IFile* getFile( const CUL::FS::Path& filePath )
{
    auto file = CUL::FS::FileFactory::createRegularFileRawPtr( filePath );
    return nullptr;
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
    if(  WinEventType::CLOSE == type )
    {
        closeApp();
    }
}

void closeApp()
{
    g_oglw->stopRenderingLoop();
    g_sdlw->stopEventLoop();
}

#if _MSC_VER
#pragma warning( pop )
#endif