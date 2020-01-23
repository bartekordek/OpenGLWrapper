#pragma once

#include "libopenglwrapper/IShader.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IShaderFactory
{
public:
    IShaderFactory() = default;
    virtual ~IShaderFactory() = default;

    virtual IShader* createShader( const IFile& filePath ) = 0;

protected:
private:
    IShaderFactory( const IShaderFactory& arg ) = delete;
    IShaderFactory& operator=( const IShaderFactory& rhv ) = delete;
};

NAMESPACE_END( LOGLW )