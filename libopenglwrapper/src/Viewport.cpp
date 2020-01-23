#include "libopenglwrapper/Viewport.hpp"
#include "CUL/STL_IMPORTS/STD_utility.hpp"

using namespace LOGLW;

Viewport::Viewport()
{
}

Viewport::Viewport( const Viewport& val ):
    m_center( val.m_center ),
    m_eye( val.m_eye ),
    m_up( val.m_up ),
    m_size( val.m_size ),
    m_fov( val.m_fov ),
    m_zNear( val.m_zNear ),
    m_zFar( val.m_zFar )
{
}

Viewport::Viewport( Viewport&& val ):
    m_center( std::move( val.m_center ) ),
    m_eye( std::move( val.m_eye ) ),
    m_up( std::move( val.m_up ) ),
    m_size( std::move( val.m_size ) ),
    m_fov( val.m_fov ),
    m_zNear( val.m_zNear ),
    m_zFar( val.m_zFar )
{
}

Cfloat Viewport::getAspectRatio() const
{
    return m_size.getWidth() * 1.0f / m_size.getHeight() * 1.0f;
}

Viewport& Viewport::operator=( const Viewport& rhv )
{
    if( this != &rhv )
    {
        m_center = rhv.m_center;
        m_eye = rhv.m_eye;
        m_up = rhv.m_up;
        m_size = rhv.m_size;
        m_fov = rhv.m_fov;
        m_zNear = rhv.m_zNear;
        m_zFar = rhv.m_zFar;
    }
    return *this;
}

Viewport& Viewport::operator=( Viewport&& rhv )
{
    if( this != &rhv )
    {
        m_center = std::move( rhv.m_center );
        m_eye = std::move( rhv.m_eye );
        m_up = std::move( rhv.m_up );
        m_size = std::move( rhv.m_size );
        m_fov = rhv.m_fov;
        m_zNear = rhv.m_zNear;
        m_zFar = rhv.m_zFar;
    }
    return *this;
}

void Viewport::setSize( const Size2Du& size )
{
    m_size = size;
}

void Viewport::setCenter( const Pos3Df& pos )
{
    m_center = pos;
}

void Viewport::setEyePos( const Pos3Df& pos )
{
    m_eye = pos;
}

void Viewport::setUp( const Pos3Df& pos )
{
    m_up = pos;
}

void Viewport::setZnear( Cdouble val )
{
    m_zNear = val;
}

void Viewport::setZfar( Cdouble val )
{
    m_zFar = val;
}

void Viewport::setFov( Cfloat val )
{
    m_fov = val;
}

const Size2Du& Viewport::getSize() const
{
    return m_size;
}

Cdouble Viewport::getLeft() const
{
    return m_center.getX() - m_size.getWidth() / 2.0;
}

Cdouble Viewport::getRight() const
{
    return m_center.getX() + m_size.getWidth() / 2.0;
}

Cdouble Viewport::getTop() const
{
    return m_center.getY() + m_size.getHeight() / 2.0;
}

Cdouble Viewport::getBottom() const
{
    return m_center.getY() - m_size.getHeight() / 2.0;
}

Cdouble Viewport::getZnear() const
{
    return m_zNear;
}

Cdouble Viewport::getZfar() const
{
    return m_zFar;
}

Cfloat Viewport::getFov() const
{
    return m_fov;
}

const Pos3Df& Viewport::getCenter() const
{
    return m_center;
}

const Pos3Df & Viewport::getEye() const
{
    return m_eye;
}

const Pos3Df & Viewport::getUp() const
{
    return m_up;
}

Viewport::~Viewport()
{
}