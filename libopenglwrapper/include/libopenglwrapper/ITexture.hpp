#pragma once

#include "libopenglwrapper/Import.hpp"

#include "CUL/Graphics/SimpleSize2D.hpp"

NAMESPACE_BEGIN( LOGLW )

using TexSize = CUL::Graphics::SimpleSize2D<unsigned>;
using TexID = unsigned int;

class LIBOPENGLWRAPPER_API ITexture
{
public:
    ITexture();
    virtual ~ITexture();

    virtual void render() = 0;

    virtual const TexSize& getSize() const = 0;

    virtual const TexID getID() const = 0;

protected:
private:
    ITexture( const ITexture& arg ) = delete;
    ITexture& operator=( const ITexture& rhv ) = delete;
};

NAMESPACE_END( LOGLW )