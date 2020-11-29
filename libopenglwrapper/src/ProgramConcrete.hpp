#pragma once

#include "libopenglwrapper/IProgram.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/STL_IMPORTS/STD_thread.hpp"

NAMESPACE_BEGIN( LOGLW )

class ProgramConcrete final:
    public IProgram
{
public:
    ProgramConcrete( IUtility* utility, IShaderFactory& sf );
    ~ProgramConcrete();

protected:
private:
    using AttribKey = unsigned int;
    using AttribMap = std::map<CUL::String, AttribKey>;

    void setAttrib( const String& name, const char* value ) override;
    void setAttrib( const String& name, Cfloat value ) override;
    void setAttrib( const String& name, Cunt value ) override;
    void setAttrib( const String& name, Cint value ) override;

    String getAttributeStr( const String& name ) override;
    float getAttributeF( const String& name ) override;
    unsigned int getAttributeUi( const String& name ) override;
    int getAttributeI( const String& name ) override;

    void attachShader( IShader* shader ) override;
    void dettachShader( IShader* shader = nullptr ) override;

    void link() override;
    void enable() override;
    void disable() override;
    void validate() override;
    IShader* createShader( const Path& path ) override;

    void render() override;

    AttribKey getAttribLocation( const String& name ) const;
    const ShaderList& getShaderList() const override;

    void bufferData( const std::vector<float>& data, const BufferTypes type ) override;

    unsigned int getProgramId() override;

    IVAO* createVao() override;

    IUtility* m_utility = nullptr;

    unsigned int m_dataBufferId = 0;
    unsigned int m_id = 0;

    IShaderFactory& m_sf;
    std::vector<IVAO*> m_vaoList;

    AttribMap m_attribMap;
    ShaderList m_attachedShaders;
    std::mutex m_operationMutex;

private: // Deleted
    ProgramConcrete( const ProgramConcrete& arg ) = delete;
    ProgramConcrete( ProgramConcrete&& arg ) = delete;
    ProgramConcrete& operator=( const ProgramConcrete& rhv ) = delete;
    ProgramConcrete& operator=( ProgramConcrete&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )