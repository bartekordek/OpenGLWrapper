#include "libopenglwrapper/Program.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/Filesystem/FileFactory.hpp"

using namespace LOGLW;

Program::Program( IUtility* utility ):
    m_id( utility->createProgram() )
{
}

// VertexArray* Program::createVao()
// {
//     VertexArray* vao = nullptr;
//     //auto vao = IVAO::createVAO();
//     //m_vaoList.push_back( vao );
//     return vao;
// }

void Program::setAttrib( const String&, const char* )
{
    // TODO
}

void Program::setAttrib( const String&, float )
{
}

void Program::setAttrib( const String&, unsigned )
{
}

void Program::setAttrib( const String&, int )
{
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

Shader* Program::createShader( const CUL::FS::Path& )
{
    //auto result = m_sf.createShader( path );
    //attachShader( result );
    //link();
    //return result;
    CUL::Assert::simple( false, "TODO: IMPLEMENT!" );
    return nullptr;
}

void Program::render()
{
    enable();
    // for( const auto& vao: m_vaoList )
    //{
    //    //vao->
    //}
}

//Program::AttribKey Program::getAttribLocation(
//    const String& name ) const
//{
//    Program::AttribKey result;
//
//    auto it = m_attribMap.find( name );
//    if( it == m_attribMap.end() )
//    {
//        result = getUtility()->getAttribLocation( m_id, name );
//    }
//    else
//    {
//        result = it->second;
//    }
//
//    return result;
//}

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