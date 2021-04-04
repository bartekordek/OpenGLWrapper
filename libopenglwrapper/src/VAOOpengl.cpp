#include "VAOOpengl.hpp"
#include "libopenglwrapper/IVertexBuffer.hpp"
#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VAOOpengl::VAOOpengl()
{
}

void VAOOpengl::setId( BuffIDType id )
{
    m_id = id;
}

BuffIDType VAOOpengl::getId() const
{
    return m_id;
}

void VAOOpengl::addVBO( IVertexBuffer* vbo )
{
    m_vbos[ vbo->getId() ] = vbo;
}

VAOOpengl::~VAOOpengl()
{
}