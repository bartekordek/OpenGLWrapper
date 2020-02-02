#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "CUL/String.hpp"

NAMESPACE_BEGIN( LOGLW )

using CsStr = CUL::CsStr;
using Cunt = const unsigned int;
using Cint = const int;
using Cfloat = const float;

class LIBOPENGLWRAPPER_API IProgram
{
public:
    IProgram() = default;
    virtual ~IProgram() = default;

    virtual void setAttrib( CsStr& name, const char* value ) = 0;
    virtual void setAttrib( CsStr& name, Cfloat value ) = 0;
    virtual void setAttrib( CsStr& name, Cunt value ) = 0;
    virtual void setAttrib( CsStr& name, Cint value ) = 0;

    virtual CsStr getAttributeStr( CsStr& name ) = 0;
    virtual Cfloat getAttributeF( CsStr& name ) = 0;
    virtual Cunt getAttributeUi( CsStr& name ) = 0;
    virtual Cint getAttributeI( CsStr& name ) = 0;

    virtual void attachShader( const IShader* shader ) = 0;
    virtual void link() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void validate() = 0;

    virtual Cunt getProgramId() const = 0;

protected:
private:
    IProgram( const IProgram& arg ) = delete;
    IProgram& operator=( const IProgram& rhv ) = delete;
};

NAMESPACE_END( LOGLW )