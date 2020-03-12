#pragma once

#include "libopenglwrapper/Import.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Pos3D.hpp"
#include "CUL/Graphics/Size2D.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size2Du = CUL::Graphics::Size2Du;
using Pos2Di = CUL::Graphics::Pos2Di;
using Pos3Di = CUL::Graphics::Pos3Di;
using Pos3Df = CUL::Graphics::Pos3Df;
using Cunt = const unsigned int;
using Cdouble = const double;

class LIBOPENGLWRAPPER_API Viewport final
{
public:
    Viewport();
    Viewport( const Viewport& val );
    Viewport( Viewport&& val );

    Cfloat getAspectRatio() const;

    Viewport& operator=( const Viewport& rhv );
    Viewport& operator=( Viewport&& rhv );

    void setSize( const Size2Du& size );
    void setCenter( const Pos3Df& pos );
    void setEyePos( const Pos3Df& pos );
    void setUp( const Pos3Df& pos );
    void setZnear( Cdouble val );
    void setZfar( Cdouble val );
    void setFov( Cfloat val );

    const Size2Du& getSize() const;
    Cdouble getLeft() const;
    Cdouble getRight() const;
    Cdouble getTop() const;
    Cdouble getBottom() const;
    Cdouble getZnear() const;
    Cdouble getZfar() const;
    Cfloat getFov() const;

    const Pos3Df& getCenter() const;
    const Pos3Df& getEye() const;
    const Pos3Df& getUp() const;

    ~Viewport();

protected:
private:
    Pos3Df m_center = { 0.0, 1.0, 0.0 };
    Pos3Df m_eye;
    Pos3Df m_up;
    Size2Du m_size;
    float m_fov = 90.0f;
    double m_zNear = 1.0;
    double m_zFar = -1.0;
};

NAMESPACE_END( LOGLW )