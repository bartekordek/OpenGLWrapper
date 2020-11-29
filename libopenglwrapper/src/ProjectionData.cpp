#include "libopenglwrapper/ProjectionData.hpp"
#include "CUL/STL_IMPORTS/STD_utility.hpp"
#include "libopenglwrapper/ProjectionData.hpp"

using namespace LOGLW;

ProjectionData::ProjectionData()
{
}

ProjectionData::ProjectionData( const ProjectionData& val ):
    CUL::GUTILS::IOnChange(),
    m_center( val.m_center ),
    m_eye( val.m_eye ),
    m_up( val.m_up ),
    m_size( val.m_size ),
    m_fov( val.m_fov ),
    m_zNear( val.m_zNear ),
    m_zFar( val.m_zFar ),
    m_left( val.m_left ),
    m_right( val.m_right ),
    m_top( val.m_top ),
    m_bottom( val.m_bottom ),
    m_projectionType( val.m_projectionType )
{
}

ProjectionData::ProjectionData( ProjectionData&& val ):
    m_center( std::move( val.m_center ) ),
    m_eye( std::move( val.m_eye ) ),
    m_up( std::move( val.m_up ) ),
    m_size( std::move( val.m_size ) ),
    m_fov( val.m_fov ),
    m_zNear( val.m_zNear ),
    m_zFar( val.m_zFar ),
    m_left( val.m_left ),
    m_right( val.m_right ),
    m_top( val.m_top ),
    m_bottom( val.m_bottom ),
    m_projectionType( val.m_projectionType )
{
}

float ProjectionData::getAspectRatio() const
{
    return static_cast<float>(m_size.getWidth()) / static_cast<float>(m_size.getHeight());
}

ProjectionData& ProjectionData::operator=( const ProjectionData& rhv )
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
        m_left = rhv.m_left;
        m_right = rhv.m_right;
        m_top = rhv.m_top;
        m_bottom = rhv.m_bottom;
        m_projectionType = rhv.m_projectionType;
    }
    return *this;
}

ProjectionData& ProjectionData::operator=( ProjectionData&& rhv )
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
        m_left = rhv.m_left;
        m_right = rhv.m_right;
        m_top = rhv.m_top;
        m_bottom = rhv.m_bottom;
        m_projectionType = rhv.m_projectionType;
    }
    return *this;
}

void ProjectionData::setSize( const Size2Di& sizeArg )
{
    m_size = sizeArg;

    m_left = m_center.x - 0.5f * (float) m_size.getWidth();
    m_right = m_center.x + 0.5f * (float) m_size.getWidth();

    m_bottom = m_center.y - 0.5f * (float) m_size.getHeight();
    m_top = m_center.y + 0.5f * (float) m_size.getHeight();
}

void ProjectionData::setCenter( const Pos3Df& pos )
{
    m_center = pos;

    m_left = m_center.x - (float) m_size.getWidth() / 2.0f;
    m_right = m_center.x + (float) m_size.getWidth() / 2.0f;

    m_bottom = m_center.y - (float) m_size.getHeight() / 2.0f;
    m_top = m_center.y + (float) m_size.getHeight() / 2.0f;
}

void ProjectionData::setEyePos( const Pos3Df& pos )
{
    m_eye = pos;
    //m_zNear = pos.z;
    onChange();
}

void ProjectionData::setUp( const Pos3Df& pos )
{
    m_up = pos;
    onChange();
}

void ProjectionData::setZnear( const float val )
{
    m_zNear = val;
    //m_eye.z = val;
    onChange();
}

void ProjectionData::setZfar( const float val )
{
    m_zFar = val;
    onChange();
}

void ProjectionData::setFov( const float val )
{
    m_fov = val;
    onChange();
}

const Size2Di& ProjectionData::getSize() const
{
    return m_size;
}

float ProjectionData::getLeft() const
{
    return m_left;
}

float ProjectionData::getRight() const
{
    return m_right;
}

float ProjectionData::getTop() const
{
    return m_top;
}

float ProjectionData::getBottom() const
{
    return m_bottom;
}

float ProjectionData::getZnear() const
{
    return m_zNear;
}

float ProjectionData::getZfar() const
{
    return m_zFar;
}

float ProjectionData::getFov() const
{
    return m_fov;
}

const Pos3Df& ProjectionData::getCenter() const
{
    return m_center;
}

const Pos3Df& ProjectionData::getEye() const
{
    return m_eye;
}

const Pos3Df& ProjectionData::getUp() const
{
    return m_up;
}

ProjectionData::~ProjectionData()
{
}