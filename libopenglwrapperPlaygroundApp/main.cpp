#include "MatrixStack.hpp"
#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "SDL2Wrapper/ISDL2Wrapper.hpp"
#include "CUL/Math/Axis.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/STL_IMPORTS/STD_iostream.hpp"

using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using Color = CUL::Graphics::ColorS;
using WinEventType = SDL2W::WindowEvent::Type;
using ShaderFile = CUL::FS::IFile;
template <typename TYPE> using DumbPtr = CUL::GUTILS::DumbPtr<TYPE>;
using FF = CUL::FS::FileFactory;
using Rect = CUL::Graphics::Rect3Di;
using Cfloat = CUL::Cfloat;
using Pos3Df = CUL::Graphics::Pos3Df;

DumbPtr<SDL2W::ISDL2Wrapper> g_sdlw;
GLWrap g_oglw;
LOGLW::MatrixStack matrixStack;
Color red( 1.0f, 0.0f, 0.0f, 1.0f );
Color yellow( 1.0f, 1.0f, 0.0f, 1.0f );
GLfloat angle = 0.0f;
DumbPtr<LOGLW::ITriangle> triangle;
ShaderFile* vertexShaderFile = nullptr;
ShaderFile* fragmentShaderFile = nullptr;
SDL2W::WindowData windowData;
float objectZ = 0.0f;
LOGLW::Viewport viewport;
Pos3Df eyePos;

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
    windowData.withOpenGL = true;

    g_sdlw = SDL2W::createSDL2Wrapper();
    g_sdlw->init( windowData );

    g_oglw = LOGLW::createOpenGLWrapper( g_sdlw );

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

    CUL::FS::Path shadersDir( "../libopenglwrapper/shaders/" );
    vertexShaderFile = FF::createRegularFileRawPtr( shadersDir + "vertexShader.vert" );
    fragmentShaderFile = FF::createRegularFileRawPtr( shadersDir + "fragmentShader.frag" );

    vertexShaderFile->load( true );
    fragmentShaderFile->load( true );

    auto vs = sf->createShader( *vertexShaderFile );
    auto fs = sf->createShader( *fragmentShaderFile );

    auto program = pf->createProgram();

    program->attachShader( vs );
    program->attachShader( fs );
    program->link();
    program->validate();
    g_oglw->setProjectionType( LOGLW::ProjectionType::PERSPECTIVE );

    auto window = g_sdlw->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ) );
    const auto& winSize = window->getSize();
    viewport.setSize( winSize );
    eyePos.z = 50.0f;
    viewport.setEyePos( eyePos );
    viewport.setCenter( Pos3Df( 0.0f, 0.0f, 0.0f ) );
    viewport.setUp( Pos3Df( 0.0f, 1.0f, 0.0f ) );
    g_oglw->setViewPort( viewport );
}

void drawTriangle( const Color& color, Cfloat size = 2.0f );

void renderScene()
{
    const float z = 0.0f;
    const float size = 64.0f;
    matrixStack.push();
        glRotatef( angle, 0.0f, 0.0f, 1.0f );
        glTranslatef( 0.0f, 32.0f, objectZ );
        drawTriangle( red, size );
        glRotatef( 180, 0.0f, 0.0f, 1.0f );
        drawTriangle( yellow, size );
    matrixStack.pop();
    //glTranslatef( -1.0f, -1.0f, objectZ ); // Center.

    angle += 0.8f;
}

void drawTriangle( const Color& color, Cfloat size )
{
    glColor4f( color.getRF(), color.getGF(), color.getBF(), color.getAF() );
    glBegin( GL_TRIANGLES );
        glVertex3f(  size, -size, 0.0f );
        glVertex3f( -size, -size, 0.0f );
        glVertex3f( -size,  size, 0.0f );
    glEnd();

    //glBegin( GL_QUADS );
    //    glColor3f( 0.0f, 1.0f, 1.0f );
    //    glVertex3f( -50.0f, -50.0f, 0.0f );
    //    glVertex3f( 50.0f, -50.0f, 0.0f );
    //    glVertex3f( 50.0f, 50.0f, 0.0f );
    //    glVertex3f( -50.0f, 50.0f, 0.0f );
    //glEnd();
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
    }
    else if( keyName == "S" )
    {
        objectZ += deltaZ;
    }
    else if( keyName == "U" )
    {
        auto val = viewport.getZfar();
        viewport.setZfar( val + delta );
        g_oglw->setViewPort( viewport );
    }
    else if( keyName == "J" )
    {
        auto val = viewport.getZfar();
        viewport.setZfar( val - delta );
        g_oglw->setViewPort( viewport );
    }
    else if( keyName == "I" )
    {
        eyePos.z += 2.0f;
        g_oglw->setEyePos( eyePos );
    }
    else if( keyName == "K" )
    {
        eyePos.z -= 2.0f;
        g_oglw->setEyePos( eyePos );
    }
    else if( keyName == "P" )
    {
        static bool toggle = true;
        if( toggle == true )
        {
            std::cout << "Changing projection to ortographic.\n";
            g_oglw->setProjectionType( LOGLW::ProjectionType::ORTO );
        }
        else
        {
            std::cout << "Changing projection to PERSPECTIVE.\n";
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