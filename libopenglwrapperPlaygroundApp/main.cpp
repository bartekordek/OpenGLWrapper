#include "MatrixStack.hpp"
#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "CUL/Graphics/Color.hpp"

using Color = CUL::Graphics::ColorS;

void changeSize( int w, int h );
void renderScene( void );
void resetTransformations();
void drawTriangle( const Color& color );

GLfloat angle = 0.0f;

Color red( 1.0f, 0.0f, 0.0f, 1.0f );
Color blue( 0.0f, 0.0f, 1.0f, 1.0f );

LOGLW::MatrixStack ms;


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4514 )
#pragma warning( disable: 5039 )
#endif
int main( int argc, char** argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( 800, 400 );
    glutInitWindowSize( 320, 320 );
    glutCreateWindow( "Generic Window name." );
    glutDisplayFunc( renderScene );
    // glutReshapeFunc( changeSize );
    glutIdleFunc( renderScene );
    changeSize( 320, 240 );
    glutMainLoop();
    return 0;
}
#ifdef _MSC_VER
#pragma warning( pop )
#endif

void changeSize( int w, int h )
{
    if( h == 0 )
    {
        h = 1;
    }

    if( w == 0 )
    {
        w = 1;
    }

    GLfloat ratio = (GLfloat) ( w * 1.0 / h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, w, h );
    gluPerspective( 90, ratio, 1, 100 );
}

void renderScene( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    resetTransformations();
    ms.push();
        //glTranslatef( 2.0f, 2.0f, 0.0f );
        glRotatef( angle, 0.0f, 0.0f, 1.0f );
        glTranslatef( 2.0f, 2.0f, 0.0f );
        drawTriangle( red );
        glRotatef( 180, 0.0f, 0.0f, 1.0f );
        drawTriangle( blue );
    ms.pop();
    //ms.push();
    //auto z = 4.0f;
    //glTranslatef( 4.0f, 0.0f, z );
    //glRotatef( angle, 0.0f, 1.0f, 0.0f );
    //drawTriangle();
    //ms.pop();

    angle += 0.2f;

    glutSwapBuffers();
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