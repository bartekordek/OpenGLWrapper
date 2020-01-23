#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IMovable.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/Filesystem/IFile.hpp"

NAMESPACE_BEGIN( LOGLW )

using IFile = CUL::FS::IFile;

class LIBOPENGLWRAPPER_API IObject:
    public IRenderable,
    public IMovable
{
public:
    IObject() = default;
    virtual ~IObject() = default;

    virtual void addShader( const IFile& shaderFile, IShaderFactory* sf ) = 0;

protected:
private:
    IObject( const IObject& value ) = delete;
    IObject& operator=( const IObject& value ) = delete;
};

NAMESPACE_END( LOGLW )