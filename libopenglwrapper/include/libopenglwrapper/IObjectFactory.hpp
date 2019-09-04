#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/Primitives/ITriangle.hpp"
#include "libopenglwrapper/Primitives/IRect.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IObjectFactory
{
public:
    IObjectFactory() = default;
    virtual ~IObjectFactory() = default;

    virtual IRect* createRect() = 0;
    virtual ITriangle* createTriangle() = 0;

protected:
private:
    IObjectFactory( const IObjectFactory& arg ) = delete;
    IObjectFactory& operator=( const IObjectFactory& rhv ) = delete;

};

NAMESPACE_END( LOGLW )