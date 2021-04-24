#include "libopenglwrapper/IndexBuffer.hpp"

using namespace LOGLW;


IndexBuffer::IndexBuffer()
{

}

void IndexBuffer::loadData(std::vector<unsigned>& data)
{
    m_data = std::move( data );

    m_id = getUtility()->generateAndBindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );

    auto indicesSize = sizeof( m_data[0] ) * m_data.size();

    getUtility()->bufferData( m_data,BufferTypes::ELEMENT_ARRAY_BUFFER );

}

IndexBuffer::~IndexBuffer()
{

}