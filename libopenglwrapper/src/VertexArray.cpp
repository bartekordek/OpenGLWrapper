#include "libopenglwrapper/VertexArray.hpp"

#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VertexArray::VertexArray()
{
}

BuffIDType VertexArray::getId() const
{
    return m_bufferId;
}

void VertexArray::addVBO( VertexBuffer* )
{
}

void VertexArray::addIndexBuffer( std::vector<unsigned>& )  // IndexBuffer*
{
}

void VertexArray::addVertexBuffer( std::vector<float>& vertices )
{
    std::lock_guard<std::mutex> guard( m_vbosMtx );
    m_vboDataToPrepare.emplace_back( std::move( vertices ) );
}

void VertexArray::render()
{
    if ( m_currentTask == TaskType::CREATE_VAO )
    {
        m_bufferId = IUtilityUser::getUtility()->generateBuffer(
            LOGLW::BufferTypes::VERTEX_ARRAY );
        m_currentTask = TaskType::NONE;
    }
    else if ( m_currentTask == TaskType::ADD_VBO )
    {
        bind();
        std::lock_guard<std::mutex> guard( m_vbosMtx );
        while( !m_vboDataToPrepare.empty() )
        {
            auto vboData = m_vboDataToPrepare.back();

            auto vbo = new VertexBuffer( vboData );

            m_vbos.emplace_back( vbo );

            getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, vbo->getId() );
            //getUtility()->vertexAttribPointer( 0, 3, LOGLW::DataType::FLOAT, false, 3 * sizeof( float ), nullptr );

            m_vboDataToPrepare.pop_back();
        }

        unbind();
        m_currentTask = TaskType::RENDER;
    }

    if ( m_currentTask == TaskType::RENDER )
    {
        bind();

        for ( size_t i = 0; i < m_shaderPrograms.size(); ++i )
        {
            m_shaderPrograms[i]->enable();
        }

        unbind();
    }
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