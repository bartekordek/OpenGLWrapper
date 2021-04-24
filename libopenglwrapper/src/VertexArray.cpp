#include "libopenglwrapper/VertexArray.hpp"

#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VertexArray::VertexArray():
    m_bufferId( IUtilityUser::getUtility()->generateBuffer(LOGLW::BufferTypes::VERTEX_ARRAY) )
{
}

BuffIDType VertexArray::getId() const
{
    return m_bufferId;
}

void VertexArray::addVBO(VertexBuffer* )
{

}

IndexBuffer* const VertexArray::addIndexBuffer(std::vector<unsigned>& )
{
    return nullptr;
}

VertexBuffer* const VertexArray::addVertexBuffer(std::vector<float>& vertices)
{
    auto vb = m_vbos.emplace_back( new VertexBuffer() ).get();
    vb->loadData( vertices );
    return vb;
}

void VertexArray::render()
{
    bind();

    for ( size_t i = 0; i < m_shaderPrograms.size(); ++i )
    {
        m_shaderPrograms[i]->enable();
    }

    unbind();
}

void VertexArray::bind()
{
    getUtility()->bindBuffer( LOGLW::BufferTypes::VERTEX_ARRAY, m_bufferId );
}

void VertexArray::unbind()
{
    getUtility()->bindBuffer( LOGLW::BufferTypes::VERTEX_ARRAY, 0 );
}

VertexArray::~VertexArray()
{
    release();
}

void VertexArray::release()
{

}