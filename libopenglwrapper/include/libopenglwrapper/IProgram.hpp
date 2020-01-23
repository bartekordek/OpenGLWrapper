#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "CUL/String.hpp"

NAMESPACE_BEGIN( LOGLW )

using CsStr = CUL::CsStr;
using Cunt = const unsigned int;

class LIBOPENGLWRAPPER_API IProgram
{
public:
    IProgram() = default;
    virtual ~IProgram() = default;

    virtual void setUniform( CsStr& name, const char* value ) = 0;
    virtual CsStr getUniform( CsStr& name ) = 0;
    virtual CsStr getAttribute( CsStr& name ) = 0;

    virtual void attachShader( const IShader* shader ) = 0;
    virtual void link() = 0;
    virtual void userProgram() = 0;
    virtual void validate() = 0;

    virtual Cunt getProgramId() const = 0;

protected:
private:
    IProgram( const IProgram& arg ) = delete;
    IProgram& operator=( const IProgram& rhv ) = delete;
};

NAMESPACE_END( LOGLW )