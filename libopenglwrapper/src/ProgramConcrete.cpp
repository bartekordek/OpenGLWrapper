#include "ProgramConcrete.hpp"
#include "VAOOpengl.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/Filesystem/FileFactory.hpp"

using namespace LOGLW;

ProgramConcrete::ProgramConcrete( IUtility* utility, IShaderFactory& sf ):
    m_utility( utility ),
    m_id( utility->createProgram() ),
    m_sf( sf )
{
}


// VertexArray* ProgramConcrete::createVao()
// {
//     VertexArray* vao = nullptr;
//     //auto vao = IVAO::createVAO();
//     //m_vaoList.push_back( vao );
//     return vao;
// }


void ProgramConcrete::setAttrib( const String&, const char* )
{
    //TODO
}

void ProgramConcrete::setAttrib( const String& , Cfloat  )
{
}

void ProgramConcrete::setAttrib( const String& , Cunt  )
{
}

void ProgramConcrete::setAttrib( const String& , Cint  )
{
}

String ProgramConcrete::getAttributeStr( const String&  )
{
    return String();
}

float ProgramConcrete::getAttributeF( const String&  )
{
    return Cfloat();
}

unsigned int ProgramConcrete::getAttributeUi( const String&  )
{
    return Cunt();
}

int ProgramConcrete::getAttributeI( const String&  )
{
    return Cint();
}

void ProgramConcrete::attachShader( Shader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    m_utility->attachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = shader;
}

void ProgramConcrete::dettachShader( Shader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    m_utility->dettachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = nullptr;
}

void ProgramConcrete::link()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->linkProgram( m_id );
}

void ProgramConcrete::enable()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->useProgram( m_id );
}

void ProgramConcrete::disable()
{
    m_utility->useProgram( 0 );
}

void ProgramConcrete::validate()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->validateProgram( m_id );
}

Shader* ProgramConcrete::createShader( const CUL::FS::Path& path )
{
    auto result = m_sf.createShader( path );
    attachShader( result );
    link();
    return result;
}

void ProgramConcrete::render()
{
    //for( const auto& vao: m_vaoList )
    //{
    //    //vao->
    //}
}

ProgramConcrete::AttribKey ProgramConcrete::getAttribLocation( const String& name ) const
{
    ProgramConcrete::AttribKey result;

    auto it = m_attribMap.find( name );
    if( it == m_attribMap.end() )
    {
        result = m_utility->getAttribLocation( m_id, name );
    }
    else
    {
        result = it->second;
    }

    return result;
}

const ShaderList& ProgramConcrete::getShaderList() const
{
    return m_attachedShaders;
}

// TODO
void ProgramConcrete::bufferData( std::vector<float>&, const BufferTypes )
{
}

unsigned int ProgramConcrete::getProgramId()
{
    return m_id;
}

ProgramConcrete::~ProgramConcrete()
{
    m_utility->removeProgram( m_id );
    m_id = 0;
}