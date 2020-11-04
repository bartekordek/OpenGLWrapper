#include "UtilConcrete.hpp"
#include "libopenglwrapper/Viewport.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"

#include "IMPORT_glew.hpp"
#include "IMPORT_SDL_opengl.hpp"
#include "ImportFreeglut.hpp"

#include "CUL/STL_IMPORTS/STD_iostream.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_sstream.hpp"

using namespace LOGLW;

const CUL::String enumToString( const GLenum val );
const GLuint toGluint( Cunt value );

UtilConcrete::UtilConcrete( CUL::CULInterface* culInterface ):
    m_culInterface( culInterface ),
    m_logger( culInterface->getLogger() )
{
}

// TODO: Remove:
#if _MSC_VER
#pragma warning( push )
#pragma warning( disable:4100 )
#endif
void UtilConcrete::setProjection( const ProjectionData& ) const
{

}
void UtilConcrete::setViewport( const Viewport& viewport ) const
{
    glViewport(
        viewport.pos.getX(),
        viewport.pos.getY(),
        viewport.size.getWidth(),
        viewport.size.getHeight() );
}
#if _MSC_VER
#pragma warning( pop )
#endif

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

void UtilConcrete::setOrthogonalPerspective( const ProjectionData& vp ) const
{
    const auto left = vp.getLeft();
    const auto right = vp.getRight();
    const auto bottom = vp.getBottom();
    const auto top = vp.getTop();
    const auto zNear = vp.getZnear();
    const auto zFar = vp.getZfar();

// glOrtho - multiply the current matrix with an orthographic matrix
//left, right
//    Specify the coordinates for the leftand right vertical clipping planes.

//bottom, top
//    Specify the coordinates for the bottomand top horizontal clipping planes.

//nearVal, farVal
//    Specify the distances to the nearerand farther depth clipping planes.These values are negative if the plane is to be behind the viewer.


//glOrtho describes a transformation that produces a parallel projection.The current matrix( see glMatrixMode ) is multiplied by this matrixand the result replaces the current matrix, as if glMultMatrix were called with the following matrix as its argument :

//2 right - left 0 0 t x 0 2 top - bottom 0 t y 0 0 - 2 farVal - nearVal t z 0 0 0 1
//    where

//    t x = -right + left right - left
//    t y = -top + bottom top - bottom
//    t z = -farVal + nearVal farVal - nearVal
//    Typically, the matrix mode is GL_PROJECTION, and left bottom - nearVal and right top - nearVal specify the points on the near clipping plane that are mapped to the lower left and upper right corners of the window, respectively, assuming that the eye is located at( 0, 0, 0 ). - farVal specifies the location of the far clipping plane.Both nearVal and farVal can be either positive or negative.

//    Use glPushMatrix and glPopMatrix to save and restore the current matrix stack.

    glOrtho(
        left, // left
        right, // right
        bottom, // bottom
        top, // top
        zNear, // near
        zFar // far
    );
}

void UtilConcrete::setPerspectiveProjection( const ProjectionData& vp ) const
{
    static auto fov = vp.getFov();
    static auto ar = vp.getAspectRatio();
    static auto zNear = vp.getZnear();
    static auto zFar = vp.getZfar();
    gluPerspective( fov, ar, zNear, zFar );
}

void UtilConcrete::lookAt( const ProjectionData& vp ) const
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
    log( "UtilConcrete::createProgram, programId = " + String( programId ) );

    if( 0 == programId )
    {
        const GLenum err = glGetError();
        assert(
            GL_NO_ERROR == programId,
            "Error creating program, error numer: " + CUL::String( err ) );
        return 0;
    }

    return programId;
}

void UtilConcrete::removeProgram( Cunt programId ) const
{
    log( "UtilConcrete::removeProgram, programId = " + String( programId ) );
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
        assert( false, shaderCompilationErrorMessage );
    }

    return id;
}

void UtilConcrete::assertOnProgramError( Cunt programId, Cunt val ) const
{
    GLint result = 0;
    glGetProgramiv( programId, val, &result );
    if( GL_FALSE == result )
    {
        GLchar eLog[1024] = { 0 };
        glGetProgramInfoLog( programId, sizeof( eLog ), nullptr, eLog );
        CUL::String message = "Error on " + enumToString( val ) + std::string( eLog );
        assert( false, message );
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

const ShaderTypes UtilConcrete::getShaderType( const CUL::String& fileExtension ) const
{
    /*
    .vert - a vertex shader
    .tesc - a tessellation control shader
    .tese - a tessellation evaluation shader
    .geom - a geometry shader
    .frag - a fragment shader
    .comp - a compute shader
    */
    if( fileExtension.equals( "frag" ) || fileExtension.equals( ".frag" ) )
    {
        return static_cast<ShaderTypes>( GL_FRAGMENT_SHADER );
    }
    else if( fileExtension.equals( "vert" ) || fileExtension.equals( ".vert" ) )
    {
        return static_cast<ShaderTypes>( GL_VERTEX_SHADER );
    }
    else if( fileExtension.equals( "geom" ) || fileExtension.equals( ".geom" ) )
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

    const GLenum err = glGetError();
    assert(
        GL_NO_ERROR == err,
        "Error creating program, error numer: " + CUL::String( err ) );
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

ContextInfo UtilConcrete::initContextVersion( SDL2W::IWindow* window, Cunt major, Cunt minor ) const
{
    ContextInfo result;

    result.glContext = SDL_GL_CreateContext( *window );
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
    const auto glStringVersion = glGetString( GL_VERSION );
    result.glVersion = glStringVersion;

    if( major >= 3 )
    {
        auto error = glewInit();
        CUL::Assert::simple(
            GLEW_OK == error,
            "GLEW error: " +
            CUL::String( reinterpret_cast<const char*>( glewGetErrorString( error ) ) +
                         result.glVersion ) );
    }
    return result;
}

void UtilConcrete::destroyContext( ContextInfo& context )
{
    SDL_GL_DeleteContext( context.glContext ); // This is basically void* !
    context.glContext = nullptr;
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

void UtilConcrete::translate( const float x, const float y, const float z )
{
    glTranslatef( x, y, z );
}

void UtilConcrete::rotate( const float angle, const float x, const float y, const float z )
{
    glRotatef( angle, x, y, z );
}

void UtilConcrete::draw( const QuadF& quad, const ColorS& color )
{
    glBegin( GL_QUADS );
        auto red = color.getRF();
        auto green = color.getGF();
        auto blue = color.getBF();
        auto alpha = color.getAF();
        glColor4f( red, green, blue, alpha );
        glVertex3f( quad.p1.getX(), quad.p1.getY(), quad.p1.getZ() );
        glVertex3f( quad.p2.getX(), quad.p2.getY(), quad.p2.getZ() );
        glVertex3f( quad.p3.getX(), quad.p3.getY(), quad.p3.getZ() );
        glVertex3f( quad.p4.getX(), quad.p4.getY(), quad.p4.getZ() );
    glEnd();
}

void UtilConcrete::draw( const QuadF& quad, const std::array<ColorS, 4>& color )
{
    glBegin( GL_QUADS );
        glColor4f( color[0].getRF(), color[ 0 ].getGF(), color[ 0 ].getBF(), color[ 0 ].getAF() );
        glVertex3f( quad.p1.getX(), quad.p1.getY(), quad.p1.getZ() );

        glColor4f( color[ 1 ].getRF(), color[ 1 ].getGF(), color[ 1 ].getBF(), color[ 1 ].getAF() );
        glVertex3f( quad.p2.getX(), quad.p2.getY(), quad.p2.getZ() );
        
        glColor4f( color[ 2 ].getRF(), color[ 2 ].getGF(), color[ 2 ].getBF(), color[ 2 ].getAF() );
        glVertex3f( quad.p3.getX(), quad.p3.getY(), quad.p3.getZ() );
        
        glColor4f( color[ 3 ].getRF(), color[ 3 ].getGF(), color[ 3 ].getBF(), color[ 3 ].getAF() );
        glVertex3f( quad.p4.getX(), quad.p4.getY(), quad.p4.getZ() );
    glEnd();
}


void UtilConcrete::draw( const TriangleF& triangle, const ColorS& color )
{
    glBegin( GL_TRIANGLES );
        glColor4f( color.getRF(), color.getGF(), color.getBF(), color.getAF() );
        glVertex3f( triangle.p1.getX(), triangle.p1.getY(), triangle.p1.getZ() );
        glVertex3f( triangle.p2.getX(), triangle.p2.getY(), triangle.p2.getZ() );
        glVertex3f( triangle.p3.getX(), triangle.p3.getY(), triangle.p3.getZ() );
    glEnd();
}

void UtilConcrete::draw( const TriangleF& quad, const std::array<ColorS, 4>& color )
{
    glBegin( GL_TRIANGLES );
        glColor4f( color[ 0 ].getRF(), color[ 0 ].getGF(), color[ 0 ].getBF(), color[ 0 ].getAF() );
        glVertex3f( quad.p1.getX(), quad.p1.getY(), quad.p1.getZ() );
        
        glColor4f( color[ 1 ].getRF(), color[ 1 ].getGF(), color[ 1 ].getBF(), color[ 1 ].getAF() );
        glVertex3f( quad.p2.getX(), quad.p2.getY(), quad.p2.getZ() );
        
        glColor4f( color[ 2 ].getRF(), color[ 2 ].getGF(), color[ 2 ].getBF(), color[ 2 ].getAF() );
        glVertex3f( quad.p3.getX(), quad.p3.getY(), quad.p3.getZ() );
    glEnd();
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

void bufferDataImpl( const void* data, const GLenum type, const GLsizeiptr dataSize );

void UtilConcrete::bufferData( const std::vector<unsigned int>& data, const BufferTypes type  ) const
{

    bufferDataImpl(
        data.data(),
        static_cast<GLenum>( type ),
        static_cast<GLsizeiptr>( data.size() * sizeof( float ) ) );
}

void UtilConcrete::bufferData( const std::vector<float>& data, const BufferTypes type  ) const
{
    bufferDataImpl(
        data.data(),
        static_cast<GLenum>( type ),
        static_cast<GLsizeiptr>( data.size() * sizeof( float ) ) );
}

void bufferDataImpl( const void* data, const GLenum target, const GLsizeiptr dataSize )
{
/*
Creates and initializes a buffer object's data store
*/
        glBufferData(
            target,// Specifies the target to which the buffer object is bound for glBufferData.
            dataSize,// Specifies the size in bytes of the buffer object's new data store.
            data,// Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
            GL_STATIC_DRAW// usage - Specifies the expected usage pattern of the data store. 
        );
/*
target:
GL_ARRAY_BUFFER - Vertex attributes
GL_ATOMIC_COUNTER_BUFFER - Atomic counter storage
GL_COPY_READ_BUFFER - Buffer copy source
GL_COPY_WRITE_BUFFER - Buffer copy destination
GL_DISPATCH_INDIRECT_BUFFER - Indirect compute dispatch commands
GL_DRAW_INDIRECT_BUFFER - Indirect command arguments
GL_ELEMENT_ARRAY_BUFFER - Vertex array indices
GL_PIXEL_PACK_BUFFER - Pixel read target
GL_PIXEL_UNPACK_BUFFER - Texture data source
GL_QUERY_BUFFER - Query result buffer
GL_SHADER_STORAGE_BUFFER - Read-write storage for shaders
GL_TEXTURE_BUFFER - Texture data buffer
GL_TRANSFORM_FEEDBACK_BUFFER - Transform feedback buffer
GL_UNIFORM_BUFFER - Uniform block storage

usage - GL_STREAM_DRAW,
        GL_STREAM_READ,
        GL_STREAM_COPY,
        GL_STATIC_DRAW,
        GL_STATIC_READ,
        GL_STATIC_COPY,
        GL_DYNAMIC_DRAW,
        GL_DYNAMIC_READ, or
        GL_DYNAMIC_COPY.

https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml:
Description
glBufferData and glNamedBufferData create a new data store for a buffer object.
In case of glBufferData, the buffer object currently bound to target is used.
For glNamedBufferData, a buffer object associated with ID specified by the
caller in buffer will be used instead.

While creating the new storage, any pre-existing data store is deleted. The new data store is created with the specified size in bytes and usage. If data is not NULL, the data store is initialized with data from this pointer. In its initial state, the new data store is not mapped, it has a NULL mapped pointer, and its mapped access is GL_READ_WRITE.

usage is a hint to the GL implementation as to how a buffer object's data store will be accessed. This enables the GL implementation to make more intelligent decisions that may significantly impact buffer object performance. It does not, however, constrain the actual usage of the data store. usage can be broken down into two parts: first, the frequency of access (modification and usage), and second, the nature of that access. The frequency of access may be one of these:

STREAM
The data store contents will be modified once and used at most a few times.

STATIC
The data store contents will be modified once and used many times.

DYNAMIC
The data store contents will be modified repeatedly and used many times.

The nature of access may be one of these:

DRAW
The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.

READ
The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.

COPY
The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.

Notes
If data is NULL, a data store of the specified size is still created, but its contents remain uninitialized and thus undefined.

Clients must align data elements consistently with the requirements of the client platform, with an additional base-level requirement that an offset within a buffer to a datum comprising N bytes be a multiple of N.

The GL_ATOMIC_COUNTER_BUFFER target is available only if the GL version is 4.2 or greater.

The GL_DISPATCH_INDIRECT_BUFFER and GL_SHADER_STORAGE_BUFFER targets are available only if the GL version is 4.3 or greater.

The GL_QUERY_BUFFER target is available only if the GL version is 4.4 or greater.

Errors
GL_INVALID_ENUM is generated by glBufferData if target is not one of the accepted buffer targets.
GL_INVALID_ENUM is generated if usage is not GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.
GL_INVALID_VALUE is generated if size is negative
GL_INVALID_OPERATION is generated by glBufferData if the reserved buffer object name 0 is bound to target.
GL_INVALID_OPERATION is generated by glNamedBufferData if buffer is not the name of an existing buffer object.
GL_INVALID_OPERATION is generated if the GL_BUFFER_IMMUTABLE_STORAGE flag of the buffer object is GL_TRUE.
GL_OUT_OF_MEMORY is generated if the GL is unable to create a data store with the specified size

*/
}

Cunt UtilConcrete::generateAndBindBuffer( const BufferTypes bufferType, const int size ) const
{
    const auto bufferId = generateBuffer( bufferType, size );
    bindBuffer( bufferType, bufferId );
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

void UtilConcrete::bindBuffer( IVAO* vao ) const
{
    bindBuffer( BufferTypes::VERTEX_ARRAY, vao->getId() );
}

void UtilConcrete::bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const
{
    if( BufferTypes::VERTEX_ARRAY == bufferType )
    {
/*
glBindVertexArray binds the vertex array object with name array. array is the
name of a vertex array object previously returned from a call to
glGenVertexArrays, or zero to break the existing vertex array object binding.

If no vertex array object with name array exists, one is created when array is
first bound. If the bind is successful no change is made to the state of the
vertex array object, and any previous vertex array object binding is broken.
*/
        glBindVertexArray(  static_cast<GLuint>( bufferId ) );
    }
    else
    {
/*
glBindBuffer binds a buffer object to the specified buffer binding point.
Calling glBindBuffer with target set to one of the accepted symbolic constants
and buffer set to the name of a buffer object binds that buffer object name to
the target. If no buffer object with name buffer exists, one is created with
that name. When a buffer object is bound to a target, the previous binding for
that target is automatically broken.

Buffer object names are unsigned integers. The value zero is reserved, but there
is no default buffer object for each buffer object target. Instead, buffer set
to zero effectively unbinds any buffer object previously bound, and restores
client memory usage for that buffer object target (if supported for that
target). Buffer object names and the corresponding buffer object contents are
local to the shared object space of the current GL rendering context; two
rendering contexts share buffer object names only if they explicitly enable
sharing between contexts through the appropriate GL windows interfaces functions.
*/
        glBindBuffer( static_cast<GLenum>( bufferType ), bufferId );
    }
}

//TODO: Remove type?
Cunt UtilConcrete::generateBuffer( const BufferTypes bufferType, const int size ) const
{
    GLuint bufferId = 0;
    if ( BufferTypes::VERTEX_ARRAY == bufferType )
    {
        glGenVertexArrays( size, &bufferId );
    }
    else
    {
        glGenBuffers( size, &bufferId );
    }

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
/*
Enable or disable a generic vertex attribute array.
glEnableVertexAttribArray and glEnableVertexArrayAttrib enable the generic vertex
attribute array specified by index. glEnableVertexAttribArray uses currently bound vertex
array object for the operation, whereas glEnableVertexArrayAttrib updates state of the
vertex array object with ID vaobj.
*/
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

void UtilConcrete::log( const String& text,
                        const CUL::LOG::Severity severity ) const
{
    assert( m_logger != nullptr,
            "Logger utility is unninitialized inside of UtilConcrete." );
    m_logger->log( text, severity );
}

void UtilConcrete::assert( const bool value, const CUL::String& message ) const
{
    CUL::Assert::simple( value, message );
}

void UtilConcrete::setDepthTest( const bool enabled ) const
{
    if( enabled )
    {
        glEnable( GL_DEPTH_TEST );
    }
    else
    {
        glDisable( GL_DEPTH_TEST );
    }
}

UtilConcrete::~UtilConcrete()
{
}