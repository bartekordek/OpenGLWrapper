#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/STL_IMPORTS/STD_thread.hpp"

NAMESPACE_BEGIN( LOGLW )

class ProgramConcrete final:
    public IProgram
{
public:
    ProgramConcrete( IUtility* utility );
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

    void attachShader( IShader* shader ) override;
    void dettachShader( IShader* shader = nullptr ) override;

    void link() override;
    void enable() override;
    void disable() override;
    void validate() override;

    const AttribKey getAttribLocation( CsStr& name ) const;
    const ShaderList& getShaderList() const override;

    void bufferData( const std::vector<float>& data, const BufferTypes type ) override;

    Cunt getProgramId() const override;

    IUtility* m_utility = nullptr;
    unsigned int m_dataBufferId = 0;
    unsigned int m_id = 0;
    AttribMap m_attribMap;
    ShaderList m_attachedShaders;
    std::mutex m_operationMutex;

private: // Deleted
    ProgramConcrete( const ProgramConcrete& arg ) = delete;
    ProgramConcrete& operator=( const ProgramConcrete& rhv ) = delete;
};

NAMESPACE_END( LOGLW )