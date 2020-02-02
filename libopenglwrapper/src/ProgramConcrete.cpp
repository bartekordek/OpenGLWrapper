#include "ProgramConcrete.hpp"
#include "OpenGL_3_Utils.hpp"

using namespace LOGLW;

ProgramConcrete::ProgramConcrete():
    m_id( OGLUTILS::createProgram() )
{
}

ProgramConcrete::~ProgramConcrete()
{
    OGLUTILS::removeProgram( m_id );
    m_id = 0;
}

void ProgramConcrete::setAttrib(
    CsStr&,
    const char* )
{
    //TODO
}

void ProgramConcrete::setAttrib( CsStr & , Cfloat  )
{
}

void ProgramConcrete::setAttrib( CsStr & , Cunt  )
{
}

void ProgramConcrete::setAttrib( CsStr & , Cint  )
{
}

CsStr ProgramConcrete::getAttributeStr( CsStr &  )
{
    return CsStr();
}

Cfloat ProgramConcrete::getAttributeF( CsStr &  )
{
    return Cfloat();
}

Cunt ProgramConcrete::getAttributeUi( CsStr &  )
{
    return Cunt();
}

Cint ProgramConcrete::getAttributeI( CsStr &  )
{
    return Cint();
}

void ProgramConcrete::attachShader( const IShader* shader )
{
    auto shaderId = shader->getId();
    OGLUTILS::attachShader( m_id, shaderId );
}

void ProgramConcrete::link()
{
    OGLUTILS::linkProgram( m_id );
}

void ProgramConcrete::enable()
{
    OGLUTILS::useProgram( m_id );
}

void ProgramConcrete::disable()
{
    OGLUTILS::useProgram( 0 );
}

void ProgramConcrete::validate()
{
    OGLUTILS::validateProgram( m_id );
}

const ProgramConcrete::AttribKey ProgramConcrete::getAttribLocation( CsStr & name ) const
{
    ProgramConcrete::AttribKey result;

    auto it = m_attribMap.find( name );
    if( it == m_attribMap.end() )
    {
        result = OGLUTILS::getAttributeLocation( m_id, name );
    }
    else
    {
        result = it->second;
    }

    return result;
}

Cunt ProgramConcrete::getProgramId() const
{
    return m_id;
}
