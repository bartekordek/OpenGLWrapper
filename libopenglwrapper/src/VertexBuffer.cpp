#include "libopenglwrapper/VertexBuffer.hpp"

using namespace LOGLW;
#include "libopenglwrapper/IUtility.hpp"

VertexBuffer::VertexBuffer( std::vector<float>& data, bool instantLoad )
    : m_vertices( std::move( data ) )
{
    if( instantLoad )
    {
        loadData( m_vertices );
    }
}

void VertexBuffer::render()
{
    if( m_load )
    {
        loadData( m_vertices );
    }
}

void VertexBuffer::loadData( std::vector<float>& data )
{
    release();
    m_bufferId =
        getUtility()->generateBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
    getUtility()->bufferData( data, LOGLW::BufferTypes::ARRAY_BUFFER );

    unsigned attribIndex = 0;
    unsigned numberOfComponents = 3;
    int stride = 3 * sizeof( m_vertices.at( 0 ) );
    getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                       LOGLW::DataType::FLOAT, false, stride,
                                       nullptr );
    getUtility()->enableVertexAttribArray( attribIndex );

    m_load = false;
}

unsigned VertexBuffer::getId() const
{
    return m_bufferId;
}

VertexBuffer::~VertexBuffer()
{
    release();
}

void VertexBuffer::release()
{
    getUtility()->deleteBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
}