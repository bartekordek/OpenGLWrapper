#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "libopenglwrapper/ViewPort.hpp"

#include "CUL/Graphics/Color.hpp"
#include "CUL/Graphics/Rect2D.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/String.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_utility.hpp"
#include "CUL/STL_IMPORTS/STD_array.hpp"
#include "CUL/Graphics/Pos3D.hpp"

#include "IMPORT_glew.hpp"

NAMESPACE_BEGIN( LOGLW )

using CDouble = const double;
using Cunt = const unsigned int;

using Angle = CUL::MATH::Angle;
using String = CUL::String;
using ColorS = CUL::Graphics::ColorS;
using ColorE = CUL::Graphics::ColorE;
using Pos3Dd = CUL::Graphics::Pos3Dd;

class OGLUTILS
{
public:
    using DispRect = CUL::Graphics::Rect2D<unsigned int>;

    static void resetMatrixToIdentity( const GLenum matrix );
    static void setViewPort( const Viewport& rect );
    static void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar );
    static void lookAt( const Viewport& vp );
    static void lookAt( const std::array< Pos3Dd, 3>& lookAtVec );
    static void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up );

    static const GLuint toGluint( Cunt value );
    static const GLenum getShaderType( CUL::CsStr& fileExtension );

    static Cunt createProgram();
    static void removeProgram( Cunt programId );
    static void useProgram( Cunt programId );
    static void linkProgram( Cunt programId );
    static void validateProgram( Cunt programId );

    static Cunt createShader( const IFile& shaderCode );
    static void attachShader( Cunt programId, Cunt shaderId );
    static void removeShader( Cunt shaderId );

    static String initContextVersion( Cunt major, Cunt minor );
    static void setProjectionAndModelToIdentity();
    static void clearColorAndDepthBuffer();
    static void createQuad();
    static void clearColorTo( const ColorS color );

    static std::vector<std::string> listExtensions();

protected:
private:
    OGLUTILS() = delete;
    OGLUTILS( const OGLUTILS& arg ) = delete;
    OGLUTILS( OGLUTILS&& arg ) = delete;
    OGLUTILS& operator=( const OGLUTILS& rhv ) = delete;
    OGLUTILS& operator=( OGLUTILS&& rhv ) = delete;
    ~OGLUTILS() = delete;
};

NAMESPACE_END( LOGLW )