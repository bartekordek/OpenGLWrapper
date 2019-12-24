#pragma once

#include "libopenglwrapper/IProgram.hpp"

#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/String.hpp"

#include "IMPORT_glew.hpp"

NAMESPACE_BEGIN( LOGLW )
NAMESPACE_BEGIN( OGLUTILS )

using CDouble = const double;

using Angle = CUL::Math::Angle;
using String = CUL::String;

struct ViewPortRect
{
    CUL::Graphics::Pos2Di pos;
    CUL::Graphics::Size2Du size;
};

using DispRect = CUL::Graphics::Rect2D<unsigned int>;

using Cunt = const unsigned int;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;

void resetMatrixToIdentity( const GLenum matrix );
void setViewPort( const ViewPortRect& rect );
void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble zNear, CDouble zFar );

const GLuint toGluint( Cunt value );
const GLenum getShaderType( CUL::CsStr& fileExtension );

Cunt createProgram();
void removeProgram( Cunt programId );
void useProgram( Cunt programId );
void linkProgram( Cunt programId );
void validateProgram( Cunt programId );

Cunt createShader( const IFile& shaderCode );
void attachShader( Cunt programId, Cunt shaderId );
void removeShader( Cunt shaderId );

String initContextVersion( Cunt major, Cunt minor );
void setProjectionAndModelToIdentity();
void clearColorAndDepthBuffer();
void createQuad();
void clearColorTo( const ColorS color );

void listExtensions();

NAMESPACE_END( OGLUTILS )
NAMESPACE_END( LOGLW )