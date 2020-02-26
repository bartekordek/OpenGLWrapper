#include "UtilConcrete.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"

#include "IMPORT_glew.hpp"
#include "IMPORT_SDL_opengl.hpp"
#include "ImportFreeglut.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_sstream.hpp"

using namespace LOGLW;

void assertOnProgramError( Cunt programId, const GLenum val );
const CUL::String enumToString( const GLenum val );
const GLuint toGluint( Cunt value );

void Assert( const bool value, const CUL::String& message )
{
    CUL::Assert::simple( value, message );
}

UtilConcrete::UtilConcrete()
{
}

void UtilConcrete::setViewPort( const Viewport& rect ) const
{
    const auto& pos = rect.getCenter();
    const auto& size = rect.getSize();
    glViewport(
        static_cast<GLint>( pos.getX() ),
        static_cast<GLint>( pos.getY() ),
        static_cast<GLsizei>( size.getWidth() ),
        static_cast<GLsizei>( size.getHeight() ) );
}

void UtilConcrete::setPerspective(
    const Angle& angle,
    CDouble widthToHeightRatio,
    CDouble m_zNear,
    CDouble m_zFar ) const
{
    gluPerspective(
        angle.getValueD( CUL::MATH::Angle::Type::DEGREE ),
        widthToHeightRatio,
        m_zNear,
        m_zFar );
}

void UtilConcrete::setOrthogonalPerspective( const Viewport& vp ) const
{
    const auto left = vp.getLeft();
    const auto right = vp.getRight();
    const auto bottom = vp.getBottom();
    const auto top = vp.getTop();
    const auto zNear = vp.getZnear();
    const auto zFar = vp.getZfar();

    glOrtho(
        left, // left
        right, // right
        bottom, // bottom
        top, // top
        zNear, // near
        zFar // far
    );
}

void UtilConcrete::setPerspectiveProjection( const Viewport& vp ) const
{
    gluPerspective( vp.getFov(), vp.getAspectRatio(), vp.getZnear(), vp.getZfar() );
}

void UtilConcrete::lookAt( const Viewport& vp ) const
{
    const auto& eye = vp.getEye();
    const auto& center = vp.getCenter();
    const auto& up = vp.getUp();
    gluLookAt(
        eye.getX(), eye.getY(), eye.getZ(),
        center.getX(), center.getY(), center.getZ(),
        up.getX(), up.getY(), up.getZ() );
}

void UtilConcrete::lookAt( const std::array<Pos3Dd, 3>& vec ) const
{
    lookAt(
        vec[0],
        vec[1],
        vec[2]
    );
}

void UtilConcrete::lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const
{
    gluLookAt(
        eye.x, eye.y, eye.z,
        center.x, center.y, center.z,
        up.x, up.y, up.z );
}

Cunt UtilConcrete::createProgram() const
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

void UtilConcrete::removeProgram( Cunt programId ) const
{
    glDeleteProgram( toGluint( programId ) );
    // TODO: find a correct way to check whether program was deleted.
    //assertOnProgramError( programId, GL_DELETE_STATUS );
}

void UtilConcrete::useProgram( Cunt programId ) const
{
    glUseProgram( static_cast<GLuint>( programId ) );
}

void UtilConcrete::linkProgram( Cunt programId ) const
{
    glLinkProgram( static_cast<GLuint>( programId ) );
    assertOnProgramError( programId, GL_LINK_STATUS );
}

void UtilConcrete::validateProgram( Cunt programId ) const
{
    glValidateProgram( programId );
    assertOnProgramError( programId, GL_VALIDATE_STATUS );
}

Cunt UtilConcrete::createShader( const IFile& shaderCode ) const
{
    const auto shaderType = UtilConcrete::getShaderType( shaderCode.getPath().getExtension() );
    const auto id = static_cast<Cunt>( glCreateShader( static_cast<GLenum>( shaderType ) ) );

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

const ShaderTypes UtilConcrete::getShaderType( CUL::CsStr& fileExtension ) const
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
        return static_cast<ShaderTypes>( GL_FRAGMENT_SHADER );
    }
    else if( fileExtension == "vert" || fileExtension == ".vert" )
    {
        return static_cast<ShaderTypes>( GL_VERTEX_SHADER );
    }
    else if( fileExtension == "geom" || fileExtension == ".geom" )
    {
        return static_cast<ShaderTypes>( GL_GEOMETRY_SHADER );
    }

    return static_cast<ShaderTypes>( GL_INVALID_ENUM );
}

void UtilConcrete::attachShader(
    Cunt programId,
    Cunt shaderId ) const
{
    glAttachShader(
        toGluint( programId ),
        toGluint( shaderId ) );
}

void UtilConcrete::dettachShader( Cunt programId, Cunt shaderId ) const
{
    glDetachShader(
        toGluint( programId ),
        toGluint( shaderId )
    );
}

void UtilConcrete::removeShader( Cunt shaderId ) const
{
    glDeleteShader(
        toGluint( shaderId ) );
}

const GLuint toGluint( Cunt value )
{
    return static_cast<GLuint>( value );
}

CUL::String UtilConcrete::initContextVersion( Cunt major, Cunt minor ) const
{
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
    const CUL::String contextInfo = glGetString( GL_VERSION );

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

void UtilConcrete::setAttribValue( Cint attributeLocation, Cfloat value ) const
{
    glUniform1f( static_cast<GLint>( attributeLocation ), value );
}

void UtilConcrete::setAttribValue( Cint attributeLocation, Cint value ) const
{
    glUniform1i( static_cast<GLint>( attributeLocation ), value );
}

void UtilConcrete::setAttribValue( Cint attributeLocation, Cunt value ) const
{
    glUniform1i( static_cast<GLint>( attributeLocation ), static_cast<int>( value ) );
}

void UtilConcrete::setProjectionAndModelToIdentity() const
{
    resetMatrixToIdentity( MatrixTypes::PROJECTION );
    resetMatrixToIdentity( MatrixTypes::MODELVIEW );
}

void UtilConcrete::resetMatrixToIdentity( const MatrixTypes matrix ) const
{
    glMatrixMode( static_cast<GLenum>( matrix ) );
    glLoadIdentity();
}

void UtilConcrete::clearColorAndDepthBuffer() const
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void UtilConcrete::createQuad( Cfloat scale ) const
{
    glBegin( GL_QUADS );
        glColor3f( 1.f, 1.f, 1.f );
        glVertex2f( -0.5f * scale, -0.5f * scale );
        glVertex2f(  0.5f * scale, -0.5f * scale );
        glVertex2f(  0.5f * scale,  0.5f * scale );
        glVertex2f( -0.5f * scale,  0.5f * scale );
    glEnd();
}

void UtilConcrete::clearColorTo( const ColorS color ) const
{
    glClearColor(
        static_cast<GLclampf>( color.getRF() ),
        static_cast<GLclampf>( color.getGF() ),
        static_cast<GLclampf>( color.getBF() ),
        static_cast<GLclampf>( color.getAF() ) );
}

void UtilConcrete::clearBuffer( const ClearMasks mask ) const
{
    glClear( static_cast<GLbitfield>( mask ) );
}

Cunt UtilConcrete::generateArrayBuffer( const int size ) const
{
    /*
    glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to glGenBuffers.

    Buffer object names returned by a call to glGenBuffers are not returned by subsequent calls, unless they are first deleted with glDeleteBuffers.

    No buffer objects are associated with the returned buffer object names until they are first bound by calling glBindBuffer.
    */
    GLuint bufferId = 0;
    glGenBuffers( size, &bufferId );
    glBindBuffer( GL_ARRAY_BUFFER, bufferId );
    return bufferId;
}

void UtilConcrete::bufferArrayData( const std::vector<float>& data ) const
{
    /*
   creates and initializes a buffer object's data store
   */
    const auto dataSize = static_cast<GLsizeiptr>( data.size() * sizeof( float ) );
    glBufferData(
        GL_ARRAY_BUFFER,
        dataSize,
        data.data(),
        GL_STATIC_DRAW );
}

void UtilConcrete::bufferArrayData( const float vertices[] ) const
{
    const auto kurwa1 = sizeof( vertices );
    const auto kurwa2 = sizeof( *vertices );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *vertices ), vertices, GL_STATIC_DRAW );
}

void UtilConcrete::enableVertexAttribiute(
    Cunt programId,
    const String& attribName ) const
{
    const auto attributeLocation = UtilConcrete::getAttribLocation( programId, attribName );
    glEnableVertexAttribArray( attributeLocation );
}

void UtilConcrete::disableVertexAttribiute(
    Cunt programId,
    const String& attribName ) const
{
    const auto attributeLocation = UtilConcrete::getAttribLocation( programId, attribName );
    glDisableVertexAttribArray( attributeLocation );
}

Cunt UtilConcrete::getAttribLocation(
    Cunt programId,
    const String& attribName ) const
{
    auto attribLocation = static_cast<const unsigned int>(
        glGetAttribLocation(
        programId,
        attribName.cStr() ) );

    return attribLocation;
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

std::vector<std::string> UtilConcrete::listExtensions()
{
    GLint extensionsCount = 0;
    glGetIntegerv( GL_NUM_EXTENSIONS, &extensionsCount );
    const GLubyte* extensions = glGetString( GL_EXTENSIONS );
    CUL::String wat = static_cast<const unsigned char*>( extensions );
    std::vector<std::string> extensionsVec = split( wat.string(), ' ' );
    return extensionsVec;
}


UtilConcrete::~UtilConcrete()
{
}