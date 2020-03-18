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

Cunt UtilConcrete::generateVertexArray( const int size ) const
{
    GLuint vao = 0;
    glGenVertexArrays( size, &vao );
    return vao;
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

void UtilConcrete::bufferData( const std::vector<unsigned int>& data, const BufferTypes type  ) const
{
    /*
    creates and initializes a buffer object's data store
   */
    const auto dataSize = static_cast<GLsizeiptr>( data.size() * sizeof( float ) );
    glBufferData(
        static_cast<GLenum>( type ),
        dataSize,
        data.data(),
        GL_STATIC_DRAW );
}

void UtilConcrete::bufferData( const std::vector<float>& data, const BufferTypes type  ) const
{
    /*
    creates and initializes a buffer object's data store
   */
    const auto dataSize = static_cast<GLsizeiptr>( data.size() * sizeof( float ) );
    glBufferData(
        static_cast<GLenum>( type ),
        dataSize,
        data.data(),
        GL_STATIC_DRAW );
}

Cunt UtilConcrete::generateAndBindBuffer( const BufferTypes bufferType, const int size ) const
{
    const auto bufferId = generateBuffer( bufferType, size );
    glBindBuffer( 
        static_cast<GLenum>( bufferType ),
        bufferId );
    return bufferId;
}

Cunt UtilConcrete::generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size ) const
{
    const auto ebo = generateBuffer( BufferTypes::ELEMENT_ARRAY_BUFFER, size );
    bindBuffer( BufferTypes::ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>( data.size() * sizeof( unsigned int ) ),
        data.data(),
        GL_STATIC_DRAW );

        /*
    glVertexAttribPointer - define an array of generic vertex attribute data
    index - Specifies the index of the generic vertex attribute to be modified.
    size - Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
    type - Specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by both functions. Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_INT_2_10_10_10_REV, and GL_UNSIGNED_INT_2_10_10_10_REV are accepted by glVertexAttribPointer. The initial value is GL_FLOAT.

    normalized - For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.

    stride - Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.

    pointer - Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
    */
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof( float ),
        (void*) 0 );
    return ebo;
}

void UtilConcrete::bufferData( const float vertices[] ) const
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

void UtilConcrete::unbindBuffer( const BufferTypes bufferType ) const
{
    bindBuffer( bufferType, 0 );
}

void UtilConcrete::bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const
{
    if( BufferTypes::VERTEX_ARRAY == bufferType )
    {
        glBindVertexArray(  static_cast<GLuint>( bufferId ) );
    }
    else
    {
        glBindBuffer( static_cast<GLenum>( bufferType ), bufferId );
    }
}


//TODO: Remove type?
Cunt UtilConcrete::generateBuffer( const BufferTypes, const int size ) const
{
    unsigned int bufferId = 0;
    glGenBuffers( size, &bufferId );
    return bufferId;
}

void UtilConcrete::drawElements( const PrimitiveType type, const std::vector<unsigned int>& data ) const
{
    glDrawElements(
        static_cast<GLenum>( type ) ,
        static_cast<GLsizei>( data.size() ),
        GL_UNSIGNED_INT,
        data.data() );
}

void UtilConcrete::drawElements( const PrimitiveType type, const std::vector<float>& data ) const
{
    glDrawElements(
        static_cast<GLenum>( type ) ,
        static_cast<GLsizei>( data.size() ),
        GL_FLOAT,
        data.data() );
}

void UtilConcrete::drawElementsFromLastBuffer(
    const PrimitiveType primitiveType,
    const DataType dataType,
    Cunt count ) const
{
    glDrawElements(
        static_cast<GLenum>( primitiveType ),
        static_cast<GLsizei>( count ),
        static_cast<GLenum>( dataType ),
        nullptr );
}

void UtilConcrete::drawArrays(
    const PrimitiveType primitiveType,
    Cunt first,
    Cunt count ) const
{
/*
glDrawArrays - render primitives from array data.
mode - Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
first - Specifies the starting index in the enabled arrays.
count - Specifies the number of indices to be rendered.
*/
    glDrawArrays( 
        static_cast<GLenum>( primitiveType ),
        static_cast<GLint>( first ),
        static_cast<GLsizei>( count ) );
}

void UtilConcrete::vertexAttribPointer(
    Cunt vertexAttributeId,
    Cint componentsPerVertexAttribute,
    const DataType dataType,
    Cbool normalized,
    Cint stride,
    const void* offset ) const
{
    // Define an array of generic vertex attribute data
/*
GLuint index - specifies the index of the generic vertex attribute to be modified.
GLint size - specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
GLenum type - specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for that function. The initial value is GL_FLOAT.
GLboolean normalized - for glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
GLsizei stride - specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
const void * pointer - specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
*/
    glVertexAttribPointer(
        static_cast<GLuint>( vertexAttributeId ),
        static_cast<GLint>( componentsPerVertexAttribute ),
        static_cast<GLenum>( dataType ),
        static_cast<GLboolean>( normalized ),
        static_cast<GLsizei>( stride ),
        offset );
}

void UtilConcrete::enableVertexAttribArray( Cunt attributeId ) const
{
    glEnableVertexAttribArray( static_cast<GLuint>( attributeId ) );
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