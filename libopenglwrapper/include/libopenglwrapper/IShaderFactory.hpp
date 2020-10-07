#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
#include "libopenglwrapper/IShader.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IShaderFactory:
    public IUtilityUser
{
public:
    IShaderFactory() = default;
    virtual ~IShaderFactory() = default;

    virtual IShader* createShader( IFile* filePath ) = 0;

protected:
private:
    IShaderFactory( const IShaderFactory& arg ) = delete;
    IShaderFactory& operator=( const IShaderFactory& rhv ) = delete;
};

NAMESPACE_END( LOGLW )