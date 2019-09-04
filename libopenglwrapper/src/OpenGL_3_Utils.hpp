#pragma once

#include "libopenglwrapper/IProgram.hpp"

#include "CUL/Color.hpp"

#include "IMPORT_glew.hpp"


NAMESPACE_BEGIN( LOGLW )
NAMESPACE_BEGIN( OGLUTILS )

using cunt = const unsigned int;
using ColorS = CUL::ColorS;
using ColorE = CUL::ColorE;

const GLuint toGluint( cunt value );
const GLenum getShaderType( CUL::CnstMyStr& fileExtension );

cunt createProgram();
void removeProgram( cunt programId );
void useProgram( cunt programId );

cunt createShader( const IFile& shaderCode );
void attachShader( cunt programId, cunt shaderId );
void removeShader( cunt shaderId );


void initContextVersion( cunt major, cunt minor );
void setViewport( cunt x, cunt y, cunt width, cunt height );
void setProjectionAndModelToIdentity();
void clearColorAndDepthBuffer();
void createQuad();
void clearColorTo( const ColorS color );

NAMESPACE_END( OGLUTILS )
NAMESPACE_END( LOGLW )