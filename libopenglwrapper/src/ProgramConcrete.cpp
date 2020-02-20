#include "ProgramConcrete.hpp"

using namespace LOGLW;

ProgramConcrete::ProgramConcrete( IUtility* utility ):
    m_utility( utility ),
    m_id( utility->createProgram() )
{
}

ProgramConcrete::~ProgramConcrete()
{
    m_utility->removeProgram( m_id );
    m_id = 0;
}

void ProgramConcrete::setAttrib(
    CsStr&,
    const char* )
{
    //TODO
}

void ProgramConcrete::setAttrib( CsStr& , Cfloat  )
{
}

void ProgramConcrete::setAttrib( CsStr& , Cunt  )
{
}

void ProgramConcrete::setAttrib( CsStr& , Cint  )
{
}

CsStr ProgramConcrete::getAttributeStr( CsStr&  )
{
    return CsStr();
}

Cfloat ProgramConcrete::getAttributeF( CsStr&  )
{
    return Cfloat();
}

Cunt ProgramConcrete::getAttributeUi( CsStr&  )
{
    return Cunt();
}

Cint ProgramConcrete::getAttributeI( CsStr&  )
{
    return Cint();
}

void ProgramConcrete::attachShader( IShader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    m_utility->attachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = shader;
}

void ProgramConcrete::dettachShader( IShader* shader )
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

const ProgramConcrete::AttribKey ProgramConcrete::getAttribLocation( CsStr& name ) const
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

Cunt ProgramConcrete::getProgramId() const
{
    return m_id;
}