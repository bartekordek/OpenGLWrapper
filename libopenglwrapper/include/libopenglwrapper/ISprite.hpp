#pragma once

#include "libopenglwrapper/IObject.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API ISprite:
    public IObject
{
public:
    ISprite() = default;


    virtual ~ISprite() = default;
protected:
private:

// Deleted:
    ISprite( const ISprite& arg ) = delete;
    ISprite( ISprite&& arg ) = delete;
    ISprite& operator=( const ISprite& rhv ) = delete;
    ISprite& operator=( ISprite&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )