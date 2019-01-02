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
    CnstMyStr&,
    const char* )
{
    //TODO
}

CnstMyStr ProgramConcrete::getUniform( CnstMyStr& )
{
    //TODO
    return CnstMyStr();
}

CnstMyStr ProgramConcrete::getAttribute( CnstMyStr& )
{
    //TODO
    return CnstMyStr();
}

void ProgramConcrete::attachShader( const IShader& )
{
    //TODO
}

void ProgramConcrete::link()
{
}

void ProgramConcrete::userProgram()
{
}

cunt ProgramConcrete::getProgramId() const
{
    return m_id;
}