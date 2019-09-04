#pragma once

#include "libopenglwrapper/Import.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IRenderable
{
public:
    IRenderable() = default;
    virtual ~IRenderable() = default;

    virtual void render() = 0;

protected:
private:
    IRenderable( const IRenderable& value ) = delete;
    IRenderable& operator=( const IRenderable& value ) = delete;

};

NAMESPACE_END( LOGLW )