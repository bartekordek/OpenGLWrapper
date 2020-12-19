#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IVAO.hpp"
#include "libopenglwrapper/ProjectionData.hpp"

#include "CUL/CULInterface.hpp"
#include "CUL/Filesystem/IFile.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Math/Primitives/Quad.hpp"
#include "CUL/Math/Primitives/Triangle.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_array.hpp"

NAMESPACE_BEGIN( CUL )
NAMESPACE_BEGIN( Graphics )
enum class PixelFormat: short;
NAMESPACE_END( Graphics )
NAMESPACE_END( CUL )

NAMESPACE_BEGIN( SDL2W )
class IWindow;
NAMESPACE_END( SDL2W )

NAMESPACE_BEGIN( LOGLW )

using Angle = CUL::MATH::Angle;
using String = CUL::String;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;
using Pos3Dd = CUL::Graphics::Pos3Dd;
using IFile = CUL::FS::IFile;
using QuadF = CUL::MATH::Primitives::QuadF;
using TriangleF = CUL::MATH::Primitives::TriangleF;

enum class MatrixTypes: int
{
    MODELVIEW = 5888,
    PROJECTION = 5889,
    INVALID = 0x0500
};

enum class ShaderTypes: int
{
    FRAGMENT_SHADER = 0x8B30,
    VERTEX_SHADER = 0x8B31,
    GEOMETRY_SHADER = 0x8DD9,
    INVALID = 0x0500
};

enum class ClearMasks: unsigned
{
    COLOR_BUFFER_BIT = 0x00004000
};

enum class BufferTypes: unsigned
{
    NONE = 0,
    VERTEX_ARRAY = 1,
    ARRAY_BUFFER = 0x8892,
    ELEMENT_ARRAY_BUFFER = 0x8893
};

enum class PrimitiveType: unsigned
{
    LINE_STRIP = 0x0003,
    TRIANGLES  = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006,
    QUADS = 0x0007,
    QUAD_STRIP = 0x0008
};

enum class DataType: int
{
    NONE = 0,
    BYTE = 0x1400,
    UNSIGNED_BYTE = 0x1401,
    SHORT = 0x1402,
    UNSIGNED_SHORT = 0x1403,
    INT = 0x1404,
    UNSIGNED_INT = 0x1405,
    FLOAT = 0x1406,
    DOUBLE = 0x140A
};

struct LIBOPENGLWRAPPER_API ContextInfo
{
    void* glContext = nullptr;
    String glVersion;
};

enum class TextureFilterType: short
{
    NONE = 0,
    NEAREST = 0x2600,
    LINEAR = 0x2601
};

enum class TextureParameters: short
{
    NONE = 0,
    MAG_FILTER = 0x2800,
    MIN_FILTER = 0x2801,
    WRAP_S = 0x2802,
    WRAP_T = 0x2803
};

struct LIBOPENGLWRAPPER_API TextureInfo
{
    unsigned int textureId = 0;
    int level = 0;
    CUL::Graphics::PixelFormat pixelFormat;
    int border = 0;
    DataType dataType = DataType::UNSIGNED_BYTE;
    void* data = nullptr;
    CUL::Graphics::SSize2Di size;
};

class Viewport;

class LIBOPENGLWRAPPER_API IUtility
{
public:
    IUtility();

    virtual void resetMatrixToIdentity( const MatrixTypes matrix ) const = 0;
    virtual void setProjection( const ProjectionData& rect ) const = 0;
    virtual void setViewport( const Viewport& viewport ) const = 0;
    virtual void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar ) const = 0;
    virtual void setOrthogonalPerspective( const ProjectionData& vp ) const = 0;
    virtual void setPerspectiveProjection( const ProjectionData& vp ) const = 0;
    virtual void lookAt( const ProjectionData& vp ) const = 0;
    virtual void lookAt( const std::array< Pos3Dd, 3>& lookAtVec ) const = 0;
    virtual void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const = 0;

    virtual ShaderTypes getShaderType( const CUL::String& fileExtension ) const = 0;

    virtual unsigned int createProgram()  = 0;
    virtual void removeProgram( Cunt programId ) const = 0;
    virtual void useProgram( Cunt programId ) const = 0;
    virtual void linkProgram( Cunt programId ) const = 0;
    virtual void validateProgram( Cunt programId ) const = 0;

    virtual unsigned int createShader(
        const IFile& shaderCode ) = 0;
    virtual void attachShader( Cunt programId, Cunt shaderId ) const = 0;
    virtual void dettachShader( Cunt programId, Cunt shaderId ) const = 0;
    virtual void removeShader( Cunt shaderId ) const = 0;

    virtual ContextInfo initContextVersion( SDL2W::IWindow* window, Cunt major, Cunt minor ) const = 0;
    virtual void destroyContext( ContextInfo& context ) = 0;

    virtual void setAttribValue( Cint attributeLocation, Cfloat value ) const = 0;
    virtual void setAttribValue( Cint attributeLocation, Cint value ) const = 0;
    virtual void setAttribValue( Cint attributeLocation, Cunt value ) const = 0;

    virtual void setProjectionAndModelToIdentity() const = 0;
    virtual void clearColorAndDepthBuffer() const = 0;
    virtual void createQuad( Cfloat scale = 1.0f ) const = 0;
    virtual void clearColorTo( const ColorS color ) const = 0;
    virtual void clearBuffer( const ClearMasks mask ) const = 0;

    virtual unsigned int generateVertexArray( const int size = 1 ) const = 0;

    virtual void bufferData( const CUL::MATH::Primitives::QuadF data, const BufferTypes type ) const = 0;
    virtual void bufferData( const std::vector<unsigned int>& data, const BufferTypes type ) const = 0;
    virtual void bufferData( const std::vector<float>& data, const BufferTypes type ) const = 0;
    virtual void bufferData( const float vertices[] ) const = 0;

// VAO, VBO
    virtual void setVertexArrayClientState( const bool enable ) const = 0;
    virtual unsigned int generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size = 1 ) const = 0;
    virtual unsigned int generateAndBindBuffer( const BufferTypes bufferType, const int size = 1 ) const = 0;

    virtual void enableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual void disableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual unsigned int getAttribLocation( Cunt programId, const String& attribName ) const = 0;
    virtual void unbindBuffer( const BufferTypes bufferType ) const = 0;
    virtual void bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const = 0;
    virtual void bindBuffer( IVAO* vao ) const = 0;
    virtual unsigned int generateBuffer( const BufferTypes type, const int size = 1 ) const = 0;

    virtual void drawElements( const PrimitiveType type, const std::vector<unsigned int>& data ) const = 0;
    virtual void drawElements( const PrimitiveType type, const std::vector<float>& data ) const = 0;
    virtual void drawElementsFromLastBuffer( const PrimitiveType primitiveType, const DataType dataType, Cunt count ) const = 0;
    virtual void drawArrays( const PrimitiveType primitiveType, Cunt first, Cunt count ) const = 0;
    virtual void vertexAttribPointer(
        Cunt vertexAttributeId,
        Cint componentsPerVertexAttribute,
        const DataType dataType,
        Cbool normalized,
        Cint stride,
        const void* offset = nullptr ) const = 0;
    virtual void enableVertexAttribArray( Cunt attributeId ) const = 0;
    virtual void setVertexPointer( int coordinatesPerVertex, DataType dataType, int stride, const void* data ) const = 0;



    virtual std::vector<std::string> listExtensions() = 0;

    virtual void draw( const QuadF& quad, const QuadF& texQuad ) = 0;
    virtual void draw( const QuadF& quad, const ColorS& color ) = 0;
    virtual void draw( const QuadF& quad, const std::array<ColorS, 4>& color ) = 0;

    virtual void draw( const TriangleF& triangle, const ColorS& color ) = 0;
    virtual void draw( const TriangleF& quad, const std::array<ColorS, 4>& color ) = 0;

    virtual void translate( const float x, const float y, const float z ) = 0;
    virtual void rotate( const float angle, const float x = 0.0f, const float y = 0.0f, const float z = 0.0f ) = 0;
    virtual void scale( const CUL::MATH::Vector3Df& scale ) const = 0;
    virtual void scale( const float scale ) const = 0;
    virtual void setDepthTest( const bool enabled ) const = 0;
    virtual void setBackfaceCUll( const bool enabled ) const = 0;

    // Texturing
    virtual void setTexuring( const bool enabled ) const = 0;
    virtual unsigned generateTexture() const = 0;
    virtual void bindTexture( const unsigned int textureId ) const = 0;
    virtual void setTextureParameter( const TextureParameters type, const TextureFilterType val ) const = 0;
    virtual void setTextureData( const TextureInfo& ti ) const = 0;
    virtual void freeTexture( unsigned int& textureId ) const = 0;

    virtual void matrixStackPush() = 0;
    virtual void matrixStackPop() = 0;


    virtual ~IUtility();

protected:
private:
};

NAMESPACE_END( LOGLW )