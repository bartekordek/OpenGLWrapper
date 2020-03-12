#pragma once

#include "libopenglwrapper/ViewPort.hpp"
#include "CUL/Filesystem/IFile.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_array.hpp"

NAMESPACE_BEGIN( LOGLW )

using Angle = CUL::MATH::Angle;
using String = CUL::String;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;
using Pos3Dd = CUL::Graphics::Pos3Dd;
using IFile = CUL::FS::IFile;

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
    BYTE = 0x1400,
    UNSIGNED_BYTE = 0x1401,
    SHORT = 0x1402,
    UNSIGNED_SHORT = 0x1403,
    INT = 0x1404,
    UNSIGNED_INT = 0x1405,
    FLOAT = 0x1406,
    DOUBLE = 0x140A
};

class LIBOPENGLWRAPPER_API IUtility
{
public:
    IUtility();

    virtual void resetMatrixToIdentity( const MatrixTypes matrix ) const = 0;
    virtual void setViewPort( const Viewport& rect ) const = 0;
    virtual void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar ) const = 0;
    virtual void setOrthogonalPerspective( const Viewport& vp ) const = 0;
    virtual void setPerspectiveProjection( const Viewport& vp ) const = 0;
    virtual void lookAt( const Viewport& vp ) const = 0;
    virtual void lookAt( const std::array< Pos3Dd, 3>& lookAtVec ) const = 0;
    virtual void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const = 0;

    virtual const ShaderTypes getShaderType( CUL::CsStr& fileExtension ) const = 0;

    virtual Cunt createProgram() const = 0;
    virtual void removeProgram( Cunt programId ) const = 0;
    virtual void useProgram( Cunt programId ) const = 0;
    virtual void linkProgram( Cunt programId ) const = 0;
    virtual void validateProgram( Cunt programId ) const = 0;

    virtual Cunt createShader(
        const IFile&
        shaderCode ) const = 0;
    virtual void attachShader( Cunt programId, Cunt shaderId ) const = 0;
    virtual void dettachShader( Cunt programId, Cunt shaderId ) const = 0;
    virtual void removeShader( Cunt shaderId ) const = 0;

    virtual String initContextVersion( Cunt major, Cunt minor ) const = 0;
    virtual void setAttribValue( Cint attributeLocation, Cfloat value ) const = 0;
    virtual void setAttribValue( Cint attributeLocation, Cint value ) const = 0;
    virtual void setAttribValue( Cint attributeLocation, Cunt value ) const = 0;

    virtual void setProjectionAndModelToIdentity() const = 0;
    virtual void clearColorAndDepthBuffer() const = 0;
    virtual void createQuad( Cfloat scale = 1.0f ) const = 0;
    virtual void clearColorTo( const ColorS color ) const = 0;
    virtual void clearBuffer( const ClearMasks mask ) const = 0;

    virtual Cunt generateVertexArray( const int size = 1 ) const = 0;

    virtual Cunt generateArrayBuffer( const int size = 1 ) const = 0;

    virtual void bufferData( const std::vector<unsigned int>& data, const BufferTypes type ) const = 0;
    virtual void bufferData( const std::vector<float>& data, const BufferTypes type ) const = 0;
    virtual void bufferData( const float vertices[] ) const = 0;

    virtual Cunt generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size = 1 ) const = 0;
    virtual Cunt generateAndBindBuffer( const BufferTypes bufferType, const int size = 1 ) const = 0;

    virtual void enableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual void disableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual Cunt getAttribLocation( Cunt programId, const String& attribName ) const = 0;
    virtual void unbindBuffer( const BufferTypes bufferType ) const = 0;
    virtual void bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const = 0;
    virtual Cunt generateBuffer( const BufferTypes type, const int size = 0 ) const = 0;

    virtual void drawElements( const PrimitiveType type, const std::vector<unsigned int>& data ) const = 0;
    virtual void drawElements( const PrimitiveType type, const std::vector<float>& data ) const = 0;
    virtual void drawElementsFromLastBuffer( const PrimitiveType primitiveType, const DataType dataType, Cunt count ) const = 0;

    virtual std::vector<std::string> listExtensions() = 0;

    virtual ~IUtility();

protected:
private:
};

NAMESPACE_END( LOGLW )