#include "libopenglwrapper/VertexBuffer.hpp"

using namespace LOGLW;
#include "libopenglwrapper/IUtility.hpp"

VertexBuffer::VertexBuffer( VertexBufferData& vertexData )
    : m_vertexData( std::move( vertexData ) )
{

    loadData();
}

void VertexBuffer::loadData()
{
    release();
    m_bufferId =
        getUtility()->generateBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
    getUtility()->bufferData( m_vertexData.vertices, LOGLW::BufferTypes::ARRAY_BUFFER );

    unsigned attribIndex = 0;
    unsigned numberOfComponents = 3;
    int stride = 0;  // 3 * sizeof( m_vertices.at( 0 ) );

    if( m_vertexData.containsColorData )
    {
        if( m_vertexData.containsTextureCoords )
        {
            stride = 8 * sizeof( float );
            size_t ptr = 0;
            // position attribute
            getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                               LOGLW::DataType::FLOAT, false,
                                               stride, ( void* ) ptr );
            getUtility()->enableVertexAttribArray( attribIndex );
            ++attribIndex;

            // color attribute
            getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                               LOGLW::DataType::FLOAT, false,
                                               stride, ( void* ) ptr );
            getUtility()->enableVertexAttribArray( attribIndex );
            ++attribIndex;
            //glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
            //glEnableVertexAttribArray( 1 );
            //// texture coord attribute
            //glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 6 * sizeof( float ) ) );
            //glEnableVertexAttribArray( 2 );
        }
        else
        {
            stride = 6 * sizeof( float );
            size_t ptr = 0;

            getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                               LOGLW::DataType::FLOAT, false,
                                               stride, ( void* ) ptr );
            getUtility()->enableVertexAttribArray( attribIndex );

            ++attribIndex;
            ptr = 3 * sizeof( float );
            getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                               LOGLW::DataType::FLOAT, false,
                                               stride, ( void* ) ptr );
            getUtility()->enableVertexAttribArray( attribIndex );
        }
    }
    else
    {
        getUtility()->vertexAttribPointer( attribIndex, numberOfComponents,
                                           LOGLW::DataType::FLOAT, false,
                                           stride, nullptr );
        getUtility()->enableVertexAttribArray( attribIndex );
    }

    if( m_vertexData.indices.size() )
    {
        m_indexBuffer = new IndexBuffer();
        m_indexBuffer->loadData( m_vertexData.indices );
    }
    else
    {
        // check the vertices count.
    }

    m_load = false;
}

void VertexBuffer::render()
{
    if( m_indexBuffer.get() )
    {
        getUtility()->drawElements( m_vertexData.primitiveType,
                                    m_indexBuffer->getData() );
    }
    else
    {
        // TODO! need to check if there are actual trianiangles or other types.
        getUtility()->drawArrays( m_vertexData.primitiveType, 0, 3 );
    }
}

unsigned VertexBuffer::getId() const
{
    return m_bufferId;
}

int VertexBuffer::getSize() const
{
    return 0;
}

void VertexBuffer::bind()
{
    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
}

VertexBuffer::~VertexBuffer()
{
    release();
}

void VertexBuffer::release()
{
    getUtility()->deleteBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_bufferId );
}