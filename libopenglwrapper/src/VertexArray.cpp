#include "libopenglwrapper/VertexArray.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/IBufferFactory.hpp"

using namespace LOGLW;

IBufferFactory* VertexArray::s_bufferFactory = nullptr;


void VertexArray::registerBufferFactory(IBufferFactory* bf)
{
    s_bufferFactory = bf;
}

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

void VertexArray::addIndexBuffer(std::vector<unsigned>&, const std::function<void( IndexBuffer* ibo )>&  ) //IndexBuffer*
{
}

void VertexArray::addVertexBuffer(std::vector<float>&, const std::function<void( VertexBuffer* vbo )>&) // VertexBuffer* const
{
    // auto vb = m_vbos.emplace_back( new VertexBuffer() ).get();
    // vb->loadData( vertices );
    // return vb;

    //s_bufferFactory->createVBO( callback, vertices );
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
    getUtility()->deleteBuffer( LOGLW::BufferTypes::VERTEX_ARRAY, m_bufferId );
}