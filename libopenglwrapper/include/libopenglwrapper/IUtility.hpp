#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "libopenglwrapper/ViewPort.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_array.hpp"

NAMESPACE_BEGIN( LOGLW )

using CDouble = const double;
using Cunt = const unsigned int;

using Angle = CUL::MATH::Angle;
using String = CUL::String;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;
using Pos3Dd = CUL::Graphics::Pos3Dd;

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

    virtual Cunt createShader( const IFile& shaderCode ) const = 0;
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

    virtual Cunt generateArrayBuffer( const int size = 1 ) const = 0;
    virtual void bufferData( const std::vector<float>& data ) const = 0;

    virtual void enableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual void disableVertexAttribiute( Cunt programId, const String& attribName ) const = 0;
    virtual Cunt getAttribLocation( Cunt programId, const String& attribName ) const = 0;

    virtual std::vector<std::string> listExtensions() = 0;

    virtual ~IUtility();

protected:
private:
};

NAMESPACE_END( LOGLW )