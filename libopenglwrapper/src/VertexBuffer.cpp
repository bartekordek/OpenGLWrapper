#include "libopenglwrapper/VertexBuffer.hpp"
#include "libopenglwrapper/VertexArray.hpp"

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
    getUtility()->bufferData( m_bufferId, m_vertexData.vertices, LOGLW::BufferTypes::ARRAY_BUFFER );

    unsigned attribIndex = 0;
    unsigned numberOfComponents = 3;
    int stride = 0;  // 3 * sizeof( m_vertices.at( 0 ) );

    VertexAttributePtrMeta meta;
    if( m_vertexData.containsColorData )
    {
        if( m_vertexData.containsTextureCoords )
        {
            meta.componentsPerVertexAttribute = 3;
            meta.dataType = DataType::FLOAT;
            meta.normalized = false;
            meta.stride = 8 * sizeof( float );
            meta.vao = 0;
            meta.vbo = m_bufferId;
            meta.vertexAttributeId = attribIndex++;

            // position attribute
            getUtility()->vertexAttribPointer( meta );
            getUtility()->enableVertexAttribArray( attribIndex );
            ++attribIndex;

            // color attribute
            meta.offset = (void*) (3 * sizeof( float ));
            meta.vertexAttributeId = attribIndex++;
            getUtility()->vertexAttribPointer( meta );
            getUtility()->enableVertexAttribArray( attribIndex );
            ++attribIndex;

            // texture coord attribute
            meta.offset = (void*)( 6 * sizeof( float ) );
            meta.vertexAttributeId = attribIndex++;
            getUtility()->vertexAttribPointer( meta );
            getUtility()->enableVertexAttribArray( attribIndex );

            //// position attribute
            //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) 0 );
            //glEnableVertexAttribArray( 0 );
            //// color attribute
            //glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
            //glEnableVertexAttribArray( 1 );
            //// texture coord attribute
            //glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 6 * sizeof( float ) ) );
            //glEnableVertexAttribArray( 2 );
        }
        else
        {
            if( m_vertexData.vao )
            {
                meta.componentsPerVertexAttribute = numberOfComponents;
                meta.dataType = DataType::FLOAT;
                meta.normalized = false;
                meta.stride = 6 * sizeof( float );
                meta.vao = m_vertexData.vao->getId();
                meta.vbo = m_bufferId;
                meta.vertexAttributeId = attribIndex++;

                getUtility()->vertexAttribPointer( meta );
                getUtility()->enableVertexAttribArray( attribIndex );

                meta.vertexAttributeId = attribIndex++;
                meta.offset = (void*)( 3 * sizeof( float ) );
                getUtility()->vertexAttribPointer( meta );
                getUtility()->enableVertexAttribArray( attribIndex );
            }

        }
    }
    else
    {
        meta.componentsPerVertexAttribute = numberOfComponents;
        meta.dataType = DataType::FLOAT;
        meta.normalized = false;
        meta.stride = 8 * sizeof( float );
        meta.vao = 0;
        meta.vbo = m_bufferId;
        meta.vertexAttributeId = attribIndex++;
        getUtility()->vertexAttribPointer( meta );
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
        getUtility()->bindBuffer( BufferTypes::VERTEX_ARRAY, m_vertexData.vao->getId() );
        getUtility()->bindBuffer( BufferTypes::ARRAY_BUFFER, m_bufferId );
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
