#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IMovable.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/Filesystem/IFile.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

using IFile = CUL::FS::IFile;

class LIBOPENGLWRAPPER_API IObject:
    public IRenderable,
    public IMovable
{
public:
    IObject() = default;
    virtual ~IObject() = default;

    virtual void addShader( IFile* shaderFile, IShaderFactory* sf ) = 0;

    virtual const std::vector<float> getData() const = 0;

protected:
private:
    IObject( const IObject& value ) = delete;
    IObject( IObject&& value ) = delete;
    IObject& operator=( const IObject& value ) = delete;
    IObject& operator=( IObject&& value ) = delete;
};

NAMESPACE_END( LOGLW )