#pragma once

#include "libopenglwrapper/Import.hpp"

#include "CUL/Filesystem/IFile.hpp"
#include "CUL/Filesystem/Path.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using IFile = CUL::FS::IFile;
using String = CUL::String;

class LIBOPENGLWRAPPER_API IShader
{
public:
    IShader( const IFile& shaderCode );
    virtual ~IShader() = default;

    virtual Cunt getId() const  = 0;
    virtual void useShader() const  = 0;

protected:
private:
    IShader() = delete;
    IShader( const IShader& arg ) = delete;
    IShader& operator=( const IShader& rhv ) = delete;

};

NAMESPACE_END( LOGLW )