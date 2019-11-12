#pragma once

#include "libopenglwrapper/IProgram.hpp"

#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/MyString.hpp"

#include "IMPORT_glew.hpp"

NAMESPACE_BEGIN( LOGLW )
NAMESPACE_BEGIN( OGLUTILS )

using CDouble = const double;

using Angle = CUL::Math::Angle;

struct ViewPortRect
{
    CUL::Graphics::Pos2Di pos;
    CUL::Graphics::Size2Du size;
};

using DispRect = CUL::Graphics::Rect2D<unsigned int>;

using cunt = const unsigned int;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;

void resetMatrixToIdentity( const GLenum matrix );
void setViewPort( const ViewPortRect& rect );
void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble zNear, CDouble zFar );

const GLuint toGluint( cunt value );
const GLenum getShaderType( CUL::CnstMyStr& fileExtension );

cunt createProgram();
void removeProgram( cunt programId );
void useProgram( cunt programId );
void linkProgram( cunt programId );
void validateProgram( cunt programId );

cunt createShader( const IFile& shaderCode );
void attachShader( cunt programId, cunt shaderId );
void removeShader( cunt shaderId );

CUL::MyString initContextVersion( cunt major, cunt minor );
void setProjectionAndModelToIdentity();
void clearColorAndDepthBuffer();
void createQuad();
void clearColorTo( const ColorS color );

void listExtensions();

NAMESPACE_END( OGLUTILS )
NAMESPACE_END( LOGLW )