#pragma once

#include "libopenglwrapper/Import.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size2Du = CUL::Graphics::Size2Du;
using Pos2Di = CUL::Graphics::Pos2Di;
using Cfloat = CUL::Cfloat;

class LIBOPENGLWRAPPER_API Viewport final
{
public:
    Viewport();
    Viewport( const Viewport& val );
    Viewport( Viewport&& val );

    Cfloat getAspectRatio() const;

    Viewport& operator=( const Viewport& rhv );
    Viewport& operator=( Viewport&& rhv );

    Pos2Di pos;
    Size2Du size;
    float fov = 90.0f;
    double left = 0.0;
    double bottom = 0.0;
    double top = 0.0;
    double zNear = 1.0;
    double zFar = -1.0;

    ~Viewport();

protected:
private:

};

NAMESPACE_END( LOGLW )