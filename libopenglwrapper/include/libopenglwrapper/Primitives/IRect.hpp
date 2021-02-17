#pragma once

#include "libopenglwrapper/IObject.hpp"

#include "CUL/Graphics/SimpleSize3D.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size = CUL::Graphics::SimpleSize3D<float>;

struct LIBOPENGLWRAPPER_API RectDimension
{
    Size size;
    Pos pos;

    RectDimension()
    {
    }

    RectDimension( const RectDimension& arg ):
        size( arg.size ),
        pos( arg.pos )
    {
    }

    RectDimension& operator=( const RectDimension& rhv )
    {
        if( this != &rhv )
        {
            size = rhv.size;
            pos = rhv.pos;
        }
        return *this;
    }
};

class LIBOPENGLWRAPPER_API IRect:
    public IObject
{
public:
    IRect() = default;

    virtual const Size& getSize() const = 0;

    virtual ~IRect() = default;

protected:
private:
    IRect( const IRect& arg ) = delete;
    IRect& operator=( const IRect& rhv ) = delete;

    /* RectDimension m_rectDimension;
    IShader* m_shader = nullptr;*/
};

NAMESPACE_END( LOGLW )