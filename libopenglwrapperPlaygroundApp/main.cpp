//#include "CUL/Filesystem/FileFactory.hpp"
//#include "CUL/GenericUtils/DumbPtr.hpp"
//#include "CUL/Graphics/Color.hpp"


#include "MatrixStack.hpp"
#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "SDL2Wrapper/ISDL2Wrapper.hpp"
#include "CUL/Math/Axis.hpp"
#include "CUL/GenericUtils/ConsoleUtilities.hpp"

using SDLWrap = CUL::GUTILS::DumbPtr<SDL2W::ISDL2Wrapper>;
using GLWrap = CUL::GUTILS::DumbPtr<LOGLW::IOpenGLWrapper>;
using Color = CUL::Graphics::ColorS;

SDL2W::ISDL2Wrapper* g_sdlw = nullptr;
GLWrap g_oglw;
LOGLW::MatrixStack matrixStack;
Color red( 1.0f, 0.0f, 0.0f, 1.0f );
Color blue( 0.0f, 0.0f, 1.0f, 1.0f );
GLfloat angle = 0.0f;

void afterInit();
void renderScene( void );

int main( int argc, char** argv )
{
    auto& argsInstance = CUL::GUTILS::ConsoleUtilities::getInstance();
    argsInstance.setArgs( argc, argv );

    SDLWrap sdlW = SDL2W::createSDL2Wrapper(
        SDL2W::Vector3Di( 256, 256, 0 ),
        SDL2W::Vector3Du( 640, 480, 0 ), "Test", true );
    g_sdlw = sdlW.get();
    auto window = sdlW->getMainWindow();
    window->setBackgroundColor( SDL2W::ColorS( 1.0f, 0.0f, 0.0f, 1.0f ) );

    g_oglw = LOGLW::createOpenGLWrapper( window, sdlW.get() );

    g_oglw->onInitialize( afterInit );
    g_oglw->beforeFrame( renderScene );

  //  sdlW->addKeyboardEventCallback( &onKeyBoardEvent );
  //  sdlW->registerWindowEventCallback( &onWindowEvent );

    g_oglw->startRenderingLoop();

    sdlW->runEventLoop();

    return 0;
}

void afterInit()
{
    //auto sf = g_oglw->getShaderFactory();
    //auto of = g_oglw->getObjectFactory();
    //CUL::FS::Path shadersDir( "../libopenglwrapper/shaders/" );
    //vertexShaderFile = FF::createRegularFileRawPtr( shadersDir + "vertexShader.vert" );
    //fragmentShaderFile = FF::createRegularFileRawPtr( shadersDir + "fragmentShader.frag" );

    //vertexShaderFile->load( true );
    //fragmentShaderFile->load( true );

    //triangle = of->createTriangle();

    //triangle->addShader( *vertexShaderFile.get(), sf );
    //triangle->addShader( *fragmentShaderFile.get(), sf );

    auto w = static_cast<GLsizei>( g_sdlw->getMainWindow()->getSize().getX() );
    auto h = static_cast<GLsizei>( g_sdlw->getMainWindow()->getSize().getY() );

    GLfloat ratio = (GLfloat) ( w * 1.0 / h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, w, h );
    gluPerspective( 90, ratio, 1, 100 );
}

void resetTransformations();
void drawTriangle( const Color& color );

void renderScene( void )
{
    gluLookAt(
        0.0f, 0.0f, 10.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f );
    matrixStack.push();
        //glTranslatef( 2.0f, 2.0f, 0.0f );
        glRotatef( angle, 0.0f, 0.0f, 1.0f );
        glTranslatef( 2.0f, 2.0f, 0.0f );
        drawTriangle( red );
        glRotatef( 180, 0.0f, 0.0f, 1.0f );
        drawTriangle( blue );
    matrixStack.pop();
    //ms.push();
    //auto z = 4.0f;
    //glTranslatef( 4.0f, 0.0f, z );
    //glRotatef( angle, 0.0f, 1.0f, 0.0f );
    //drawTriangle();
    //ms.pop();

    angle += 0.2f;
}

void resetTransformations()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(
        0.0f, 0.0f, 10.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f );
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

//
//using WinEventType = SDL2W::WindowEvent::Type;
//using ShaderFile = CUL::GUTILS::DumbPtr<CUL::FS::IFile>;
//
//
//using FF = CUL::FS::FileFactory;
//
//void onKeyBoardEvent( const SDL2W::IKey& key );
//void onWindowEvent( const WinEventType type );
//
//
//
//void closeApp();
//

//ShaderFile vertexShaderFile;
//ShaderFile fragmentShaderFile;
//LOGLW::ITriangle* triangle = nullptr;
//
//CUL::FS::IFile* getFile( const CUL::FS::Path& filePath );
//
//
//void changeSize( int w, int h );
//
//
//
//
//
//

//
//LOGLW::MatrixStack ms;
//
//int main( int argc, char** argv )
//{

//
//    

//    return 0;
//}

////int main( int argc, char** argv )
////{
////    glutInit( &argc, argv );
////    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
////    glutInitWindowPosition( 800, 400 );
////    glutInitWindowSize( 320, 320 );
////    glutCreateWindow( "Generic Window name." );
////    glutDisplayFunc( renderScene );
////    // glutReshapeFunc( changeSize );
////    glutIdleFunc( renderScene );
////    changeSize( 320, 240 );
////    glutMainLoop();
////    return 0;
////}
//
//void changeSize( int w, int h )
//{
//    if( h == 0 )
//    {
//        h = 1;
//    }
//
//    if( w == 0 )
//    {
//        w = 1;
//    }
//
//    GLfloat ratio = (GLfloat) ( w * 1.0 / h );
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    glViewport( 0, 0, w, h );
//    gluPerspective( 90, ratio, 1, 100 );
//}
//

//

//

//
//void onKeyBoardEvent( const SDL2W::IKey& key )
//{
//    if( key.getKeyName() == "q" || key.getKeyName() == "Q" )
//    {
//        closeApp();
//    }
//}
//
//void onWindowEvent( const WinEventType type )
//{
//    if( WinEventType::CLOSE == type )
//    {
//        closeApp();
//    }
//}
//
//void closeApp()
//{
//    g_oglw->stopRenderingLoop();
//    g_sdlw->stopEventLoop();
//}