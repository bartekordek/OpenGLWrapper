#include "MatrixStack.hpp"
#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "SDL2Wrapper/ISDL2Wrapper.hpp"
#include "CUL/Math/Axis.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/Graphics/Rect2D.hpp"

using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using Color = CUL::Graphics::ColorS;
using WinEventType = SDL2W::WindowEvent::Type;
using ShaderFile = CUL::FS::IFile;
template <typename TYPE> using DumbPtr = CUL::GUTILS::DumbPtr<TYPE>;
using FF = CUL::FS::FileFactory;
using Rect = CUL::Graphics::Rect3Di;

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
    windowData.size.setSize( 640, 480 );
    windowData.withOpenGL = true;

    g_sdlw = SDL2W::createSDL2Wrapper();
    g_sdlw->init( windowData );

    auto window = g_sdlw->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ) );

    g_oglw = LOGLW::createOpenGLWrapper( g_sdlw );

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
}

void drawTriangle( const Color& color );

void renderScene()
{
    const float z = 0.0f;
    matrixStack.push();
        glRotatef( angle, 0.0f, 0.0f, 1.0f );
        glTranslatef( 2.0f, 2.0f, objectZ );
        drawTriangle( red );
        glRotatef( 180, 0.0f, 0.0f, 1.0f );
        drawTriangle( yellow );
    matrixStack.pop();

    angle += 0.8f;
}

void drawTriangle( const Color& color )
{
    glColor4f( color.getRF(), color.getGF(), color.getBF(), color.getAF() );
    glBegin( GL_TRIANGLES );
        glVertex3f( 2.0f, -2.0f, 0.0f );
        glVertex3f( -2.0f, -2.0f, 0.0f );
        glVertex3f( -2.0f, 2.0f, 0.0f );
    glEnd();
}

void onKeyBoardEvent( const SDL2W::IKey& key )
{
    const float deltaZ = 2.0f;
    if( key.getKeyName() == "Q" )
    {
        closeApp();
    }
    else if( key.getKeyName() == "W" )
    {
        objectZ -= deltaZ;
    }
    else if( key.getKeyName() == "S" )
    {
        objectZ += deltaZ;
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