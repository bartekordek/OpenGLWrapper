#include "libopenglwrapper/VertexBuffer.hpp"

using namespace LOGLW;
#include "libopenglwrapper/IUtility.hpp"

VertexBuffer::VertexBuffer( std::vector<float>& data )
{
    loadData( data );
}

void VertexBuffer::loadData( std::vector<float>& data )
{
    release();
    m_vertices = std::move( data );

    m_bufferId =
        getUtility()->generateBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
    getUtility()->bufferData( m_vertices, LOGLW::BufferTypes::ARRAY_BUFFER );

    unsigned attribIndex = 0;
    unsigned numberOfComponents = 3;
    size_t stride = 3 * sizeof( m_vertices.at( 0 ) );
    getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                       LOGLW::DataType::FLOAT, false, stride,
                                       nullptr );
    getUtility()->enableVertexAttribArray( attribIndex );
}

VertexBuffer::~VertexBuffer()
{
    release();
}

void VertexBuffer::release()
{
    getUtility()->deleteBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
}