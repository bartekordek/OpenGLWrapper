#include "VAOOpengl.hpp"
#include "libopenglwrapper/IVBO.hpp"
#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VAOOpengl::VAOOpengl()
{
}

void VAOOpengl::setId( Cunt id )
{
    m_id = id;
}

unsigned int VAOOpengl::getId() const
{
    return m_id;
}

void VAOOpengl::addVBO( IVBO* vbo )
{
    m_vbos[ vbo->getId() ] = vbo;
}

VAOOpengl::~VAOOpengl()
{
}