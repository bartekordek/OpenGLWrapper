#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"

NAMESPACE_BEGIN( LOGLW )

class ProgramConcrete final:
    public IProgram
{
public:
    ProgramConcrete();
    ~ProgramConcrete();

protected:
private:
    using AttribKey = unsigned int;
    using AttribMap = std::map<CUL::String, AttribKey>;

    void setAttrib( CsStr& name, const char* value ) override;
    void setAttrib( CsStr& name, Cfloat value ) override;
    void setAttrib( CsStr& name, Cunt value ) override;
    void setAttrib( CsStr& name, Cint value ) override;

    CsStr getAttributeStr( CsStr& name ) override;
    Cfloat getAttributeF( CsStr& name ) override;
    Cunt getAttributeUi( CsStr& name ) override;
    Cint getAttributeI( CsStr& name ) override;

    void attachShader( const IShader* shader ) override;
    void link() override;
    void enable() override;
    void disable() override;
    void validate() override;

    const AttribKey getAttribLocation( CsStr& name ) const;

    Cunt getProgramId() const override;

    unsigned int m_id = 0;
    AttribMap m_attribMap;

private: // Deleted
    ProgramConcrete( const ProgramConcrete& arg ) = delete;
    ProgramConcrete& operator=( const ProgramConcrete& rhv ) = delete;
};

NAMESPACE_END( LOGLW )
