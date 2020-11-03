#include "libopenglwrapper/ViewPort.hpp"
#include "CUL/STL_IMPORTS/STD_utility.hpp"

using namespace LOGLW;

Viewport::Viewport()
{
}

Viewport::Viewport( const Viewport& val ):
    center( val.center ),
    eye( val.eye ),
    up( val.up ),
    size( val.size ),
    fov( val.fov ),
    zNear( val.zNear ),
    zFar( val.zFar )
{
}

Viewport::Viewport( Viewport&& val ):
    center( std::move( val.center ) ),
    eye( std::move( val.eye ) ),
    up( std::move( val.up ) ),
    size( std::move( val.size ) ),
    fov( val.fov ),
    zNear( val.zNear ),
    zFar( val.zFar )
{
}

Cfloat Viewport::getAspectRatio() const
{
    return static_cast<float>(size.getWidth()) / static_cast<float>(size.getHeight());
}

Viewport& Viewport::operator=( const Viewport& rhv )
{
    if( this != &rhv )
    {
        center = rhv.center;
        eye = rhv.eye;
        up = rhv.up;
        size = rhv.size;
        fov = rhv.fov;
        zNear = rhv.zNear;
        zFar = rhv.zFar;
    }
    return *this;
}

Viewport& Viewport::operator=( Viewport&& rhv )
{
    if( this != &rhv )
    {
        center = std::move( rhv.center );
        eye = std::move( rhv.eye );
        up = std::move( rhv.up );
        size = std::move( rhv.size );
        fov = rhv.fov;
        zNear = rhv.zNear;
        zFar = rhv.zFar;
    }
    return *this;
}

void Viewport::setSize( const Size2Du& sizeArg )
{
    size = sizeArg;
}

void Viewport::setCenter( const Pos3Df& pos )
{
    center = pos;
}

void Viewport::setEyePos( const Pos3Df& pos )
{
    eye = pos;
}

void Viewport::setUp( const Pos3Df& pos )
{
    up = pos;
}

void Viewport::setZnear( Cdouble val )
{
    zNear = val;
}

void Viewport::setZfar( Cdouble val )
{
    zFar = val;
}

void Viewport::setFov( Cfloat val )
{
    fov = val;
}

const Size2Du& Viewport::getSize() const
{
    return size;
}

Cdouble Viewport::getLeft() const
{
    return center.getX() - size.getWidth() / 2.0;
}

Cdouble Viewport::getRight() const
{
    return center.getX() + size.getWidth() / 2.0;
}

Cdouble Viewport::getTop() const
{
    return center.getY() - size.getHeight() / 2.0;
}

Cdouble Viewport::getBottom() const
{
    return center.getY() + size.getHeight() / 2.0;
}

Cdouble Viewport::getZnear() const
{
    return zNear;
}

Cdouble Viewport::getZfar() const
{
    return zFar;
}

Cfloat Viewport::getFov() const
{
    return fov;
}

const Pos3Df& Viewport::getCenter() const
{
    return center;
}

const Pos3Df & Viewport::getEye() const
{
    return eye;
}

const Pos3Df & Viewport::getUp() const
{
    return up;
}

Viewport::~Viewport()
{
}