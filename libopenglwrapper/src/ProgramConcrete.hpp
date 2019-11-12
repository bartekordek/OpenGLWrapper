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

    void setUniform( CnstMyStr& name, const char* value ) override;
    CnstMyStr getUniform( CnstMyStr& name ) override;
    CnstMyStr getAttribute( CnstMyStr& name ) override;

    void attachShader( const IShader* shader ) override;
    void link() override;
    void userProgram() override;
    void validate() override;

    virtual cunt getProgramId()const override;

protected:
private:
    ProgramConcrete( const ProgramConcrete& arg ) = delete;
    ProgramConcrete& operator=( const ProgramConcrete& rhv ) = delete;

    unsigned int m_id = 0;
};

NAMESPACE_END( LOGLW )