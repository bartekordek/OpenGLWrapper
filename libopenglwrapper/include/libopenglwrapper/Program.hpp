#pragma once

#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/Shader.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

#include "CUL/Filesystem/Path.hpp"

NAMESPACE_BEGIN( LOGLW )

using String = CUL::String;
using ShaderList = std::map<String, Shader*>;

class VertexArray;

class LIBOPENGLWRAPPER_API Program final: private IUtilityUser
{
public:
    Program( IUtility* utility );

    void setAttrib( const String& name, const char* value );
    void setAttrib( const String& name, float value );
    void setAttrib( const String& name, unsigned value );
    void setAttrib( const String& name, int value );

    String getAttributeStr( const String& name );
    float getAttributeF( const String& name );
    unsigned int getAttributeUi( const String& name );
    int getAttributeI( const String& name );

    void attachShader( Shader* shader );
    void dettachShader( Shader* shader = nullptr );
    void link();
    void enable();
    void disable();
    void validate();
    Shader* createShader( const CUL::FS::Path& path );

    void bufferData(
        std::vector<float>& data,
        const BufferTypes type );

    unsigned int getProgramId();
    const ShaderList& getShaderList() const;

    void render();

    ~Program();
protected:

private:
    using AttribKey = unsigned int;
    using AttribMap = std::map<CUL::String, AttribKey>;

    unsigned int m_dataBufferId = 0;
    unsigned int m_id = 0;

    std::vector<VertexArray*> m_vaoList;

    AttribMap m_attribMap;
    ShaderList m_attachedShaders;
    std::mutex m_operationMutex;

private:
    Program( const Program& arg ) = delete;
    Program( Program&& arg ) = delete;
    Program& operator=( const Program& rhv ) = delete;
    Program& operator=( Program&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )