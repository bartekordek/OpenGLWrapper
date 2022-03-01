#include "libopenglwrapper/Program.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/Filesystem/FileFactory.hpp"

using namespace LOGLW;

Program::Program()
{
}

bool Program::initialized() const
{
    return m_id != 0;
}

void Program::initialize()
{
    m_id = getUtility()->createProgram();
}


void Program::setAttrib( const String&, const char* )
{
    // TODO
}

void Program::setAttrib( const String& name, float value )
{
    ValueToSet task;
    task.name = name;
    task.value = value;
    pushTask( task );
}

void Program::setAttrib( const String& name, unsigned value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setAttribValue( location, value );
}

void Program::setAttrib( const String& name, int value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setAttribValue( location, value );
}

void Program::setAttrib( const String& name, bool value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setAttribValue( location, value );
}

void Program::setAttrib( const String& name, const glm::mat2& value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setUniformValue( location, value );
}

void Program::setAttrib( const String& name, const glm::mat3& value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setUniformValue( location, value );
}

void Program::setAttrib( const String& name, const glm::mat4& value )
{
    auto location = getUtility()->getUniformLocation( m_id, name );
    getUtility()->setUniformValue( location, value );
}

String Program::getAttributeStr( const String& )
{
    return String();
}

float Program::getAttributeF( const String& )
{
    return float();
}

unsigned int Program::getAttributeUi( const String& )
{
    return unsigned();
}

int Program::getAttributeI( const String& )
{
    return int();
}

void Program::attachShader( Shader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    getUtility()->attachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = shader;
}

void Program::dettachShader( Shader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    getUtility()->dettachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = nullptr;
}

void Program::link()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    getUtility()->linkProgram( m_id );
}

void Program::enable()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    getUtility()->useProgram( m_id );
}

void Program::disable()
{
    getUtility()->useProgram( 0 );
}

void Program::validate()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    getUtility()->validateProgram( m_id );
}

void Program::render()
{
    enable();
    goThroughTasks();
}

void Program::goThroughTasks()
{
    std::lock_guard<std::mutex> tasksGuard( m_operationMutex );
    while( !m_tasks.empty() )
    {
        processTask( m_tasks.front() );
        m_tasks.pop_front();
    }
}

void Program::pushTask( ValueToSet& task )
{
    std::lock_guard<std::mutex> tasksGuard( m_operationMutex );
    m_tasks.push_back( task );
}

void Program::processTask( const ValueToSet& task )
{
    {
        auto floatValue = std::get_if<float>( &task.value );
        if( floatValue )
        {
            auto location = getUtility()->getUniformLocation( m_id, task.name );
            getUtility()->setAttribValue( location, *floatValue );
            return;
        }
    }

    {
        auto unsignedValue = std::get_if<unsigned>( &task.value );
        if( unsignedValue )
        {
            auto location = getUtility()->getUniformLocation( m_id, task.name );
            getUtility()->setAttribValue( location, *unsignedValue );
            return;
        }
    }

    {
        auto intValue = std::get_if<unsigned>( &task.value );
        if( intValue )
        {
            auto location = getUtility()->getUniformLocation( m_id, task.name );
            getUtility()->setAttribValue( location, *intValue );
        }
    }
}

const ShaderList& Program::getShaderList() const
{
    return m_attachedShaders;
}

// TODO
void Program::bufferData( std::vector<float>&, const BufferTypes )
{
}

unsigned int Program::getProgramId()
{
    return m_id;
}

Program::~Program()
{
    getUtility()->removeProgram( m_id );
    m_id = 0;
}