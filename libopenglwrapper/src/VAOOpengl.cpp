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

Cunt VAOOpengl::getId() const
{
    return m_id;
}

IVBO* VAOOpengl::createVBO()
{
    auto vbo = IVBO::createVBO();
    auto vboId = getUtility()->generateAndBindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    vbo->setId( vboId );
    addVBO( vbo );
    return vbo;
}

void VAOOpengl::addVBO( IVBO* vbo )
{
    m_vbos[ vbo->getId() ] = vbo;
}

VAOOpengl::~VAOOpengl()
{
}