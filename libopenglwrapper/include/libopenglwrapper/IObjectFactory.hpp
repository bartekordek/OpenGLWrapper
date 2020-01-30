#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "libopenglwrapper/Primitives/IRect.hpp"

#include "CUL/Filesystem/IFile.hpp"

NAMESPACE_BEGIN( LOGLW )

using CsStr = CUL::CsStr;

class LIBOPENGLWRAPPER_API IObjectFactory
{
public:
    IObjectFactory() = default;
    virtual ~IObjectFactory() = default;

    virtual IRect* createRect() = 0;
    virtual Triangle* createTriangle() = 0;
    virtual IObject* createFromFile( IFile* file ) = 0;
    virtual IObject* createFromFile( const CsStr& path ) = 0;

protected:
private:
    IObjectFactory( const IObjectFactory& arg ) = delete;
    IObjectFactory( IObjectFactory&& arg ) = delete;
    IObjectFactory& operator=( const IObjectFactory& rhv ) = delete;
    IObjectFactory& operator=( IObjectFactory&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )