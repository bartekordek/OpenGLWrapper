#pragma once

#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "libopenglwrapper/IVertexArray.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/Filesystem/Path.hpp"

NAMESPACE_BEGIN( LOGLW )

using String = CUL::String;
using ShaderList = std::map<String, IShader*>;

class LIBOPENGLWRAPPER_API IProgram
{
public:
    IProgram();

    virtual void setAttrib( const String& name, const char* value ) = 0;
    virtual void setAttrib( const String& name, Cfloat value ) = 0;
    virtual void setAttrib( const String& name, Cunt value ) = 0;
    virtual void setAttrib( const String& name, Cint value ) = 0;

    virtual String getAttributeStr( const String& name ) = 0;
    virtual float getAttributeF( const String& name ) = 0;
    virtual unsigned int getAttributeUi( const String& name ) = 0;
    virtual int getAttributeI( const String& name ) = 0;

    virtual void attachShader( IShader* shader ) = 0;
    virtual void dettachShader( IShader* shader = nullptr ) = 0;
    virtual void link() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void validate() = 0;
    virtual IShader* createShader( const CUL::FS::Path& path ) = 0;

    virtual void bufferData(
        const std::vector<float>& data,
        const BufferTypes type ) = 0;

    virtual unsigned int getProgramId() = 0;
    virtual const ShaderList& getShaderList() const = 0;

    virtual IVertexArray* createVao() = 0;

    virtual void render() = 0;

    virtual ~IProgram();
protected:
private:
    IProgram( const IProgram& arg ) = delete;
    IProgram( IProgram&& arg ) = delete;
    IProgram& operator=( const IProgram& rhv ) = delete;
    IProgram& operator=( IProgram&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )