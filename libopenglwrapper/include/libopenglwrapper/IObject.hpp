#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/ITransformable.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/Filesystem/IFile.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )


class LIBOPENGLWRAPPER_API IObject:
    public IRenderable,
    public ITransformable
{
public:
    IObject();

    virtual void addShader( const CUL::FS::Path& filePath, IShaderFactory* sf );

    virtual const std::vector<float> getData() const;

    virtual ~IObject();
protected:
private:


// Deleted:
    IObject( const IObject& value ) = delete;
    IObject( IObject&& value ) = delete;
    IObject& operator=( const IObject& value ) = delete;
    IObject& operator=( IObject&& value ) = delete;
};

NAMESPACE_END( LOGLW )