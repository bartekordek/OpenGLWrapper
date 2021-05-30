#include "libopenglwrapper/VertexArray.hpp"
#include "libopenglwrapper/IndexBuffer.hpp"

#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VertexArray::VertexArray()
{
    registerTask( TaskType::CREATE_VAO );
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
    registerTask( TaskType::ADD_VBO );
    m_vboDataToPrepare.emplace_back( std::move( vertices ) );
}

void VertexArray::render()
{
    runTasks();
    bind();
    m_shaderProgram->render();
    if( !m_indexBuffers.empty() )
    {
        getUtility()->drawElements( LOGLW::PrimitiveType::TRIANGLES,
                                    m_indexBuffers.front()->getData() );
    }
    else
    {
        getUtility()->drawArrays( LOGLW::PrimitiveType::TRIANGLES, 0, 3 );
    }
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
            createVAO();
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
                bind();
                ib->loadData( indices );
                unbind();
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
                    m_shaderProgram->attachShader( shader );
                    
                    m_shadersPaths.pop();
                }
                m_shaderProgram->link();
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
        m_vboDataToPrepare.pop_back();
    }
    unbind();
}

void VertexArray::createVAO()
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