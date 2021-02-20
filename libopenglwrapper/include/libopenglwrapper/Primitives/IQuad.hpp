#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/IUtility.hpp"

#include "CUL/Graphics/SimpleSize3D.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size = CUL::Graphics::SimpleSize3D<float>;

class LIBOPENGLWRAPPER_API IQuad:
    public IObject
{
public:
    IQuad() = default;

    virtual void setValues( const QuadData& values ) = 0;
    virtual void setColor( const QuadColors& colors ) = 0;
    virtual void setColor( const ColorS& color ) = 0;

    virtual ~IQuad() = default;

protected:
private:
    IQuad( const IQuad& arg ) = delete;
    IQuad( IQuad&& arg ) = delete;
    IQuad& operator=( const IQuad& rhv ) = delete;
    IQuad& operator=( IQuad&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )