#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IMovable.hpp"
#include "CUL/Graphics/ISize3DD.hpp"

NAMESPACE_BEGIN( LOGLW )

using ISize3DD = CUL::Graphics::ISize3DD;

class LIBOPENGLWRAPPER_API IObject:
    public IRenderable,
    public IMovable
{
public:
    IObject() = default;
    virtual ~IObject() = default;

protected:
private:
    IObject( const IObject& value ) = delete;
    IObject& operator=( const IObject& value ) = delete;

};

NAMESPACE_END( LOGLW )