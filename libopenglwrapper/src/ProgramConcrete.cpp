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

void ProgramConcrete::setUniform(
    CsStr&,
    const char* )
{
    //TODO
}

CsStr ProgramConcrete::getUniform( CsStr& )
{
    //TODO
    return CsStr();
}

CsStr ProgramConcrete::getAttribute( CsStr& )
{
    //TODO
    return CsStr();
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

void ProgramConcrete::userProgram()
{
    OGLUTILS::useProgram( m_id );
}

void ProgramConcrete::validate()
{
    OGLUTILS::validateProgram( m_id );
}

Cunt ProgramConcrete::getProgramId() const 
{
    return m_id;
}