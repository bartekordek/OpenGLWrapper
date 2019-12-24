#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

class ProgramConcrete final:
    public IProgram
{
public:
    ProgramConcrete();
    ~ProgramConcrete();

    void setUniform( CsStr& name, const char* value ) override;
    CsStr getUniform( CsStr& name ) override;
    CsStr getAttribute( CsStr& name ) override;

    void attachShader( const IShader* shader ) override;
    void link() override;
    void userProgram() override;
    void validate() override;

    Cunt getProgramId() const override;

protected:
private:
    ProgramConcrete( const ProgramConcrete& arg ) = delete;
    ProgramConcrete& operator=( const ProgramConcrete& rhv ) = delete;

    unsigned int m_id = 0;
};

NAMESPACE_END( LOGLW )