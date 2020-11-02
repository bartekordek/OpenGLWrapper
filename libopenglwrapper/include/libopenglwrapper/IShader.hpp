#pragma once

#include "libopenglwrapper/Import.hpp"

#include "CUL/Filesystem/IFile.hpp"
#include "CUL/Filesystem/Path.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using IFile = CUL::FS::IFile;
using String = CUL::String;
using Path = CUL::FS::Path;

class LIBOPENGLWRAPPER_API IShader
{
public:
    IShader();

    virtual Cunt getId() const = 0;
    virtual void useShader() const = 0;
    virtual void reload() = 0;
    virtual const Path& getPath() const = 0;

    virtual ~IShader();

protected:
private:
    IShader( const IShader& arg ) = delete;
    IShader( IShader&& arg ) = delete;
    IShader& operator=( const IShader& rhv ) = delete;
    IShader& operator=( IShader&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )