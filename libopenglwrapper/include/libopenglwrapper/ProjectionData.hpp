#pragma once

#include "libopenglwrapper//Import.hpp"
#include "CUL/Graphics/Pos3D.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"

NAMESPACE_BEGIN( LOGLW )

using Pos3Di = CUL::Graphics::Pos3Di;
using Pos3Df = CUL::Graphics::Pos3Df;
using Pos2Di = CUL::Graphics::Pos2Di;
using Size2Du = CUL::Graphics::Size2Du;
using Size2Di = CUL::Graphics::Size2Di;
using Cunt = const unsigned int;
using Cdouble = const double;

class LIBOPENGLWRAPPER_API ProjectionData final
{
public:
    ProjectionData();
    ProjectionData( const ProjectionData& val );
    ProjectionData( ProjectionData&& val );

    Cfloat getAspectRatio() const;

    ProjectionData& operator=( const ProjectionData& rhv );
    ProjectionData& operator=( ProjectionData&& rhv );

    void setSize( const Size2Di& size );
    void setCenter( const Pos3Df& pos );
    void setEyePos( const Pos3Df& pos );
    void setUp( const Pos3Df& pos );
    void setZnear( Cdouble val );
    void setZfar( Cdouble val );
    void setFov( Cfloat val );

    const Size2Di& getSize() const;
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

    ~ProjectionData();
protected:
private:
    Pos3Df m_center = { 0.0, 0.0, 0.0 };
    Pos3Df m_eye;
    Pos3Df m_up;
    Size2Di m_size;
    float m_fov = 90.0f;
    double m_zNear  =  1.0;
    double m_zFar   = -1.0;
    double m_left   =  0.0;
    double m_right  =  0.0;
    double m_top    =  0.0;
    double m_bottom =  0.0;
};

NAMESPACE_END( LOGLW )