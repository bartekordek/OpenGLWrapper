#include "OpenGL_3_Utils.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "IMPORT_SDL_opengl.hpp"

#include "ImportFreeglut.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_sstream.hpp"

using namespace LOGLW;

void assertOnProgramError( Cunt programId, const GLenum val );
const CUL::String enumToString( const GLenum val );

void Assert( const bool value, const CUL::String& message )
{
    CUL::Assert::simple( value, message );
}

void OGLUTILS::setViewPort( const Viewport& rect )
{
    const auto& pos = rect.getCenter();
    const auto& size = rect.getSize();
    glViewport(
        static_cast<GLint>( pos.getX() ),
        static_cast<GLint>( pos.getY() ),
        static_cast<GLsizei>( size.getWidth() ),
        static_cast<GLsizei>( size.getHeight() ) );
}

void OGLUTILS::setPerspective(
    const Angle& angle,
    CDouble widthToHeightRatio,
    CDouble m_zNear,
    CDouble m_zFar )
{
    gluPerspective(
        angle.getValueD( CUL::MATH::Angle::Type::DEGREE ),
        widthToHeightRatio,
        m_zNear,
        m_zFar );
}

void OGLUTILS::lookAt( const Viewport& vp )
{
    const auto& eye = vp.getEye();
    const auto& center = vp.getCenter();
    const auto& up = vp.getUp();
    gluLookAt(
        eye.getX(), eye.getY(), eye.getZ(),
        center.getX(), center.getY(), center.getZ(),
        up.getX(), up.getY(), up.getZ() );
}

void OGLUTILS::lookAt( const std::array<Pos3Dd, 3>& vec )
{
    lookAt(
        vec[0],
        vec[1],
        vec[2]
    );
}

void OGLUTILS::lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up )
{
    gluLookAt(
        eye.x, eye.y, eye.z,
        center.x, center.y, center.z,
        up.x, up.y, up.z );
}

Cunt OGLUTILS::createProgram()
{
    const auto programId = static_cast<const unsigned int>(
        glCreateProgram() );

    if( 0 == programId )
    {
        const GLenum err = glGetError();
        Assert(
            GL_NO_ERROR == programId,
            "Error creating program, error numer: " + CUL::String( err ) );
        return 0;
    }

    return programId;
}

void OGLUTILS::removeProgram( Cunt programId )
{
    glDeleteProgram( toGluint( programId ) );
    // TODO: find a correct way to check whether program was deleted.
    //assertOnProgramError( programId, GL_DELETE_STATUS );
}

void OGLUTILS::useProgram( Cunt programId )
{
    glUseProgram( static_cast<GLuint>( programId ) );
}

void OGLUTILS::linkProgram( Cunt programId )
{
    glLinkProgram( static_cast<GLuint>( programId ) );
    assertOnProgramError( programId, GL_LINK_STATUS );
}

void OGLUTILS::validateProgram( Cunt programId )
{
    glValidateProgram( programId );
    assertOnProgramError( programId, GL_VALIDATE_STATUS );
}

Cunt OGLUTILS::createShader( const IFile& shaderCode )
{
    const auto shaderType = OGLUTILS::getShaderType( shaderCode.getPath().getExtension() );
    const auto id = static_cast<Cunt>( glCreateShader( shaderType ) );

    auto codeLength = static_cast<GLint>(
        shaderCode.getLinesCount() );
    glShaderSource( id, codeLength, shaderCode.getContent(), nullptr );
    glCompileShader( id );

    GLint compilationResult = 0;
    glGetShaderiv( id, GL_COMPILE_STATUS, &compilationResult );
    if( GL_FALSE == compilationResult )
    {
        GLchar eLog[1024] = { 0 };
        glGetShaderInfoLog( id, sizeof( eLog ), nullptr, eLog );
        auto errorAsString = std::string( eLog );
        CUL::String shaderCompilationErrorMessage = "Error compiling shader: " +
            errorAsString + "\n";
        shaderCompilationErrorMessage += "Shader Path: " + shaderCode.getPath().getPath() + "\n";
        Assert( false, shaderCompilationErrorMessage );
    }

    return id;
}

void assertOnProgramError( Cunt programId, const GLenum val )
{
    GLint result = 0;
    glGetProgramiv( programId, val, &result );
    if( GL_FALSE == result )
    {
        GLchar eLog[1024] = { 0 };
        glGetProgramInfoLog( programId, sizeof( eLog ), nullptr, eLog );
        CUL::String message = "Error on " + enumToString( val ) + std::string( eLog );
        Assert( false, message );
    }
}

const CUL::String enumToString( const GLenum val )
{
    switch( val )
    {
    case GL_COMPILE_STATUS: return "compile";
    case GL_LINK_STATUS: return "link";
    case GL_VALIDATE_STATUS: return "validate";
    case GL_DELETE_STATUS: return "delete";
    default:
        return "";
    }
}

const GLenum OGLUTILS::getShaderType( CUL::CsStr& fileExtension )
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
    Cunt programId,
    Cunt shaderId )
{
    glAttachShader(
        toGluint( programId ),
        toGluint( shaderId ) );
}

void OGLUTILS::removeShader( Cunt shaderId )
{
    glDeleteShader(
        toGluint( shaderId ) );
}

const GLuint OGLUTILS::toGluint( Cunt value )
{
    return static_cast<GLuint>( value );
}

CUL::String OGLUTILS::initContextVersion( Cunt major, Cunt minor )
{
    CUL::String contextInfo;
    //glutInitContextVersion( static_cast<int>( major ), static_cast<int>( minor ) );
    /*
    Context version can be only set after context creation.
    I.e. SDL: SDL_GL_DeleteContext call.
    */
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, static_cast<int>( major ) );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, static_cast<int>( minor ) );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // Possible values:
    //typedef enum
    //{
    //    SDL_GL_CONTEXT_PROFILE_CORE = 0x0001,
    //    SDL_GL_CONTEXT_PROFILE_COMPATIBILITY = 0x0002,
    //    SDL_GL_CONTEXT_PROFILE_ES = 0x0004 /**< GLX_CONTEXT_ES2_PROFILE_BIT_EXT */
    //} SDL_GLprofile;
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
    contextInfo = glGetString( GL_VERSION );

    if( major >= 3 )
    {
        auto error = glewInit();
        CUL::Assert::simple(
            GLEW_OK == error,
            "GLEW error: " +
            CUL::String( reinterpret_cast<const char*>( glewGetErrorString( error ) ) +
            contextInfo ) );
    }
    return contextInfo;
}

GLuint OGLUTILS::getAttributeLocation( Cunt programId, CsStr& attributeName )
{
    GLuint result = static_cast<GLuint>( glGetAttribLocation( programId, attributeName.cStr() ) );
    return result;
}

void OGLUTILS::setProjectionAndModelToIdentity()
{
    resetMatrixToIdentity( GL_PROJECTION );
    resetMatrixToIdentity( GL_MODELVIEW );
}

void OGLUTILS::resetMatrixToIdentity( const GLenum matrix )
{
    glMatrixMode( matrix );
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

template <typename Out>
void split( const std::string &s, char delim, Out result )
{
    std::istringstream iss( s );
    std::string item;
    while( std::getline( iss, item, delim ) )
    {
        *result++ = item;
    }
}

std::vector<std::string> split( const std::string &s, char delim )
{
    std::vector<std::string> elems;
    split( s, delim, std::back_inserter( elems ) );
    return elems;
}

std::vector<std::string> OGLUTILS::listExtensions()
{
    GLint extensionsCount = 0;
    glGetIntegerv( GL_NUM_EXTENSIONS, &extensionsCount );
    const GLubyte* extensions = glGetString( GL_EXTENSIONS );
    CUL::String wat = static_cast<const unsigned char*>( extensions );
    std::vector<std::string> extensionsVec = split( wat.string(), ' ' );
    return extensionsVec;
}
