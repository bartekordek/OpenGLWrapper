#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IMovable.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/Graphics/ISize3DD.hpp"
#include "CUL/Filesystem/IFile.hpp"

NAMESPACE_BEGIN( LOGLW )

using ISize3DD = CUL::Graphics::ISize3DD;
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