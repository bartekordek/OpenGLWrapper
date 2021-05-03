#pragma once

#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/Shader.hpp"

#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

#include "CUL/Filesystem/Path.hpp"

NAMESPACE_BEGIN( LOGLW )

using String = CUL::String;
using ShaderList = std::map<String, Shader*>;

class VertexArray;

class LIBOPENGLWRAPPER_API Program
{
public:
    Program();

    virtual void setAttrib( const String& name, const char* value ) = 0;
    virtual void setAttrib( const String& name, float value ) = 0;
    virtual void setAttrib( const String& name, unsigned value ) = 0;
    virtual void setAttrib( const String& name, int value ) = 0;

    virtual String getAttributeStr( const String& name ) = 0;
    virtual float getAttributeF( const String& name ) = 0;
    virtual unsigned int getAttributeUi( const String& name ) = 0;
    virtual int getAttributeI( const String& name ) = 0;

    virtual void attachShader( Shader* shader ) = 0;
    virtual void dettachShader( Shader* shader = nullptr ) = 0;
    virtual void link() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void validate() = 0;
    virtual Shader* createShader( const CUL::FS::Path& path ) = 0;

    virtual void bufferData(
        std::vector<float>& data,
        const BufferTypes type ) = 0;

    virtual unsigned int getProgramId() = 0;
    virtual const ShaderList& getShaderList() const = 0;

    virtual void render() = 0;

    virtual ~Program();
protected:
private:
    Program( const Program& arg ) = delete;
    Program( Program&& arg ) = delete;
    Program& operator=( const Program& rhv ) = delete;
    Program& operator=( Program&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )