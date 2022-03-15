#include "libopenglwrapper/VertexArray.hpp"
#include "libopenglwrapper/IndexBuffer.hpp"

#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VertexArray::VertexArray( bool createOnRender )
{
    if( createOnRender )
    {
        registerTask( TaskType::CREATE_VAO );
    }
    else
    {
        createVAO();
    }
}

BuffIDType VertexArray::getId() const
{
    return m_bufferId;
}

void VertexArray::addVBO( VertexBuffer* )
{
}

Program* VertexArray::getProgram()
{
    return &m_shaderProgram;
}

void VertexArray::createShader( const CUL::FS::Path& path )
{
    CUL::Assert::simple( path.exists(), "File does not exist: " + path.getPath() );
    std::lock_guard<std::mutex> guard( m_shadersMtx );
    if( m_shaderProgram.initialized() == false )
    {
        registerTask( TaskType::CREATE_PROGRAM );
    }
    m_shadersPaths.push( path );
    registerTask( TaskType::ADD_SHADER );
}

void VertexArray::addVertexBuffer( VertexBufferData& data, bool instant )
{
    if( instant )
    {
        m_vboDataToPrepare.emplace_back( std::move( data ) );
        createVBOs();
    }
    else
    {
        std::lock_guard<std::mutex> guard( m_vbosMtx );
        m_vboDataToPrepare.emplace_back( std::move( data ) );
        registerTask( TaskType::ADD_VBO );
    }
}

void VertexArray::render()
{
    runTasks();
    bind();
    m_shaderProgram.render();

    for( auto i = 0; i < m_vbosCount; ++i )
    {
        m_vbos[i]->render();
    }
    m_shaderProgram.disable();
    unbind();
}

VertexBuffer* VertexArray::getVertexBuffer()
{
    return m_vbos.front();
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
        else if( task == TaskType::CREATE_PROGRAM )
        {
            if( m_shaderProgram.initialized() == false )
            {
                m_shaderProgram.initialize();
            }
        }
        else if( task == TaskType::ADD_SHADER )
        {
            if( m_shaderProgram.initialized() == false)
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

                    auto shaderFile = getUtility()->getCUl()->getFF()->createFileFromPath( shaderPath );
                    shaderFile->load(true);
                    auto shader = new Shader( shaderFile );
                    m_shaderProgram.attachShader( shader );

                    m_shadersPaths.pop();
                }
                m_shaderProgram.link();
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
    std::lock_guard<std::mutex> guard( m_vbosMtx );
    while( !m_vboDataToPrepare.empty() )
    {
        auto vboData = m_vboDataToPrepare.back();
        vboData.vao = this;
        auto vbo = new VertexBuffer( vboData );
        m_vbos.emplace_back( vbo );
        m_vboDataToPrepare.pop_back();
        ++m_vbosCount;
    }
}

void VertexArray::createVAO()
{
    m_bufferId = IUtilityUser::getUtility()->generateBuffer(
        BufferTypes::VERTEX_ARRAY );
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