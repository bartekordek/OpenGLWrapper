#pragma once

#include "libopenglwrapper/Primitives/Triangle.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IObjectFactory
{
public:
    IObjectFactory();
    virtual ~IObjectFactory();

protected:
private:
    IObjectFactory( const IObjectFactory& arg ) = delete;
    IObjectFactory& operator=( const IObjectFactory& rhv ) = delete;

};

NAMESPACE_END( LOGLW )