#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IPoint:
    public IObject
{
public:
    IPoint() = default;

    virtual void setColor( const ColorS& color ) = 0;

    virtual ~IPoint() = default;

protected:
private:
    IPoint( const IPoint& arg ) = delete;
    IPoint( IPoint&& arg ) = delete;
    IPoint& operator=( const IPoint& rhv ) = delete;
    IPoint& operator=( IPoint&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )