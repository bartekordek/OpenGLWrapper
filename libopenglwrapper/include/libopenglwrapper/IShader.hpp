#pragma once

#include "libopenglwrapper/Import.hpp"

NAMESPACE_BEGIN( CUL )
NAMESPACE_BEGIN( FS )
class Path;
NAMESPACE_END( FS )
NAMESPACE_END( CUL )

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IShader
{
public:
    IShader();

    virtual unsigned int getId() const = 0;
    virtual void useShader() const = 0;
    virtual void reload() = 0;
    virtual const CUL::FS::Path& getPath() const = 0;

    virtual ~IShader();

protected:
private:
    IShader( const IShader& arg ) = delete;
    IShader( IShader&& arg ) = delete;
    IShader& operator=( const IShader& rhv ) = delete;
    IShader& operator=( IShader&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )