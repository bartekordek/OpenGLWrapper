#include "OpenGL_3_Utils.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "IMPORT_SDL_opengl.hpp"

#include "ImportFreeglut.hpp"

using namespace LOGLW;
using namespace OGLUTILS;

cunt OGLUTILS::createProgram()
{
    const auto programId = static_cast<const unsigned int>(
        glCreateProgram() );
    
    if( 0 == programId )
    {
        GLenum err = glGetError();
        CUL::Assert::simple(
            GL_NO_ERROR == programId,
            "Error creating program, error numer: " + CUL::MyString( err ) );
        return 0;
    }

    return cunt();
}

void OGLUTILS::removeProgram( cunt programId )
{
    glDeleteProgram( toGluint( programId ) );
    //TODO: error handling when deleting.
}

void OGLUTILS::useProgram( cunt programId )
{
    glUseProgram( static_cast<GLuint>( programId ) );
}

cunt OGLUTILS::createShader( const IFile& shaderCode )
{
    auto shaderType = OGLUTILS::getShaderType( shaderCode.getPath().getExtension() );
    auto id = static_cast< cunt >( glCreateShader( shaderType ) );

    auto codeLength = static_cast< GLint >(
        shaderCode.getLinesCount() );
    glShaderSource( id, codeLength, shaderCode.getContent(), nullptr );
    glCompileShader( id );

    GLint compilationResult = 0;
    glGetShaderiv( id, GL_COMPILE_STATUS, &compilationResult );
    if( GL_FALSE == compilationResult )
    {
        GLchar eLog[ 1024 ] = { 0 };
        glGetShaderInfoLog( id, sizeof( eLog ), nullptr, eLog );
        auto errorAsString = std::string( eLog );
        CUL::MyString shaderCompilationErrorMessage = "Error compiling shader: " +
            errorAsString + "\n";
        CUL::Assert::simple( false, shaderCompilationErrorMessage );
    }

    return id;
}

const GLenum OGLUTILS::getShaderType( CUL::CnstMyStr& fileExtension )
{
    /*
    .vert - a vertex shader
    .tesc - a tessellation control shader
    .tese - a tessellation evaluation shader
    .geom - a geometry shader
    .frag - a fragment shader
    .comp - a compute shader
    */
    if( fileExtension == "frag" || fileExtension == ".frag" )
    {
        return GL_FRAGMENT_SHADER;
    }
    else if( fileExtension == "vert" || fileExtension == ".vert" )
    {
        return GL_VERTEX_SHADER;
    }
    else if( fileExtension == "geom" || fileExtension == ".geom" )
    {
        return GL_GEOMETRY_SHADER;
    }
    return GL_INVALID_ENUM;
}

void OGLUTILS::attachShader(
    cunt programId,
    cunt shaderId )
{
    glAttachShader(
        toGluint( programId ),
        toGluint( shaderId ) );
}

void OGLUTILS::removeShader( cunt shaderId )
{
    glDeleteShader( 
        toGluint( shaderId ) );
}

const GLuint OGLUTILS::toGluint( cunt value )
{
    return static_cast< GLuint >( value );
}

void OGLUTILS::initContextVersion( cunt major, cunt minor )
{
    //glutInitContextVersion( static_cast<int>( major ), static_cast<int>( minor ) );
    /*
    Context version can be only set after context creation.
    I.e. SDL: SDL_GL_DeleteContext call.
    */
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, static_cast<int>( major ) );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, static_cast<int>( minor ) );
    auto glVersion = glGetString( GL_VERSION );

    if( major >= 3 )
    {
        auto error = glewInit();
        CUL::Assert::simple(
            GLEW_OK == error,
            "GLEW error: " +
            CUL::MyString( reinterpret_cast<const char*>( glewGetErrorString( error ) ) +
            CUL::MyString( reinterpret_cast<const char*>( glVersion ) ) ) );
    }
}

void OGLUTILS::setViewport( const DispRect& rect )
{
    glViewport( static_cast<GLint>( rect.x ),
                static_cast<GLint>( rect.y ),
                static_cast<GLsizei>( rect.width ),
                static_cast<GLsizei>( rect.height ) );
}

void OGLUTILS::setProjectionAndModelToIdentity()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void OGLUTILS::clearColorAndDepthBuffer()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void OGLUTILS::createQuad()
{
    glBegin( GL_QUADS );
        glColor3f( 1.f, 1.f, 1.f );
        glVertex2f( -0.5f, -0.5f );
        glVertex2f( 0.5f, -0.5f );
        glVertex2f( 0.5f, 0.5f );
        glVertex2f( -0.5f, 0.5f );
    glEnd();
}

void OGLUTILS::clearColorTo( const ColorS color )
{
    glClearColor(
        static_cast<GLclampf>( color.getRF() ),
        static_cast<GLclampf>( color.getGF() ),
        static_cast<GLclampf>( color.getBF() ),
        static_cast<GLclampf>( color.getAF() ) );
}
