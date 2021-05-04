#include "libopenglwrapper/VertexArray.hpp"
#include "libopenglwrapper/IndexBuffer.hpp"

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

void VertexArray::addIndexBuffer( std::vector<unsigned>& indices )
{
    m_indicesToPrepare.emplace_back( std::move( indices ) );
    registerTask( TaskType::ADD_IBO );
}

void VertexArray::createShader( const CUL::FS::Path& path )
{
    std::lock_guard<std::mutex> guard( m_shadersMtx );
    if( m_shaderProgram.get() == nullptr )
    {
        registerTask( TaskType::CREATE_PROGRAM );
    }
    m_shadersPaths.push( path );
    registerTask( TaskType::ADD_SHADER );
}

void VertexArray::addVertexBuffer( std::vector<float>& vertices )
{
    std::lock_guard<std::mutex> guard( m_vbosMtx );
    registerTask( TaskType::CREATE_VAO );
    m_vboDataToPrepare.emplace_back( std::move( vertices ) );
}

void VertexArray::render()
{
    runTasks();


    bind();

    //for ( size_t i = 0; i < m_shaderPrograms.size(); ++i )
    //{
    //    m_shaderPrograms[i]->enable();
    //}

    //m_shaderProgram->enable();
    m_shaderProgram->render();

    if( !m_indexBuffers.empty() )
    {
        getUtility()->drawElements( LOGLW::PrimitiveType::TRIANGLES, LOGLW::FloatData() );
       // glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0 );
    }
    else
    {
        getUtility()->drawArrays( LOGLW::PrimitiveType::TRIANGLES, 0, 3 );
        //glDrawArrays( GL_TRIANGLES, 0, 3 );
    }

    unbind();

}

bool VertexArray::taskIsAlreadyPlaced( TaskType tt ) const
{
    return std::find( m_tasks.begin(), m_tasks.end(), tt ) != m_tasks.end();
}

void VertexArray::runTasks()
{
    std::lock_guard<std::mutex> tasksGuard( m_tasksMtx );
    while( !m_tasks.empty() )
    {
        auto task = m_tasks.front();
        if( task == TaskType::CREATE_VAO )
        {
            createVao();
        }
        else if( task == TaskType::ADD_VBO )
        {
            createVBOs();
        }
        else if( task == TaskType::ADD_IBO )
        {
            while( !m_indicesToPrepare.empty() )
            {
                auto indices = m_indicesToPrepare.front();

                auto ib = new IndexBuffer();
                ib->loadData( indices );
                m_indexBuffers.emplace_back( ib );

                m_indicesToPrepare.pop_back();
            }
        }
        else if( task == TaskType::CREATE_PROGRAM )
        {
            m_shaderProgram = new Program( getUtility() );
        }
        else if( task == TaskType::ADD_SHADER )
        {
            if( m_shaderProgram.get() == nullptr )
            {
                if( !taskIsAlreadyPlaced( TaskType::CREATE_PROGRAM ) )
                {
                    m_tasks.push_back( TaskType::CREATE_PROGRAM );
                }
                m_tasks.push_back( TaskType::ADD_SHADER );
            }
            else
            {
                std::lock_guard<std::mutex> shadersGuard( m_shadersMtx );
                while( !m_shadersPaths.empty() )
                {
                    auto shaderPath = m_shadersPaths.front();

                    auto shaderFile = getUtility()->getCUl()->getFF()->createFileFromPath(
                            shaderPath );
                    auto shader = new Shader( shaderFile );
                    //m_shaders.emplace_back( shader );
                    m_shaderProgram->attachShader( shader );
                    m_shadersPaths.pop();
                }
            }
        }

        m_tasks.pop_front();
    }
}

void VertexArray::registerTask( TaskType taskType )
{
    std::lock_guard<std::mutex> guard( m_tasksMtx );
    if( taskType == TaskType::CREATE_PROGRAM )
    {
        if( taskIsAlreadyPlaced( taskType ) )
        {
            return;
        }
    }
    m_tasks.push_back( taskType );
}

void VertexArray::createVBOs()
{
    bind();
    std::lock_guard<std::mutex> guard( m_vbosMtx );
    while( !m_vboDataToPrepare.empty() )
    {
        auto vboData = m_vboDataToPrepare.back();

        auto vbo = new VertexBuffer( vboData );

        m_vbos.emplace_back( vbo );

        getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER,
                                  vbo->getId() );
        // getUtility()->vertexAttribPointer( 0, 3, LOGLW::DataType::FLOAT,
        // false, 3 * sizeof( float ), nullptr );

        m_vboDataToPrepare.pop_back();
    }

    unbind();
}

void VertexArray::createVao()
{
    m_bufferId = IUtilityUser::getUtility()->generateBuffer(
        LOGLW::BufferTypes::VERTEX_ARRAY );
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