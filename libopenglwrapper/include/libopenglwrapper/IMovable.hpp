#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "CUL/Graphics/IPosition3DD.hpp"

NAMESPACE_BEGIN( LOGLW )

using IPosition3DD = CUL::Graphics::IPosition3DD;

class LIBOPENGLWRAPPER_API IMovable
{
public:
    IMovable() = default;
    virtual ~IMovable() = default;

    virtual const IPosition3DD& getPos()const = 0;
    virtual void setPosition( const IPosition3DD& pos ) = 0;
    virtual void move( const IPosition3DD& moveVect ) = 0;
    virtual void returnToOrigin() = 0;

protected:
private:
    IMovable( const IMovable& value ) = delete;
    IMovable& operator=( const IMovable& value ) = delete;

};

NAMESPACE_END( LOGLW )