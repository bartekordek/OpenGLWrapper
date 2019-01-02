#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "CUL/MyString.hpp"

NAMESPACE_BEGIN( LOGLW )

using CnstMyStr = CUL::CnstMyStr;
using cunt = const unsigned int;

class LIBOPENGLWRAPPER_API IProgram
{
public:
    IProgram() = default;
    virtual ~IProgram() = default;

    virtual void setUniform( CnstMyStr& name, const char* value ) = 0;
    virtual CnstMyStr getUniform( CnstMyStr& name ) = 0;
    virtual CnstMyStr getAttribute( CnstMyStr& name ) = 0;

    virtual void attachShader( const IShader& shader ) = 0;
    virtual void link() = 0;
    virtual void userProgram() = 0;

    virtual cunt getProgramId()const = 0;

protected:
private:
    IProgram( const IProgram& arg ) = delete;
    IProgram& operator=( const IProgram& rhv ) = delete;

};

NAMESPACE_END( LOGLW )