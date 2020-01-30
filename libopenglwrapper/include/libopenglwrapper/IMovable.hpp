#pragma once

#include "libopenglwrapper/IRenderable.hpp"

#include "CUL/Graphics/IPosition3DD.hpp"
#include "CUL/Math/Point.hpp"

NAMESPACE_BEGIN( LOGLW )

using Pos = CUL::MATH::PointF;
using TranslationVector = CUL::MATH::PointF;

class LIBOPENGLWRAPPER_API IMovable
{
public:
    IMovable() = default;
    virtual ~IMovable() = default;

    virtual const Pos& getPos() const = 0;
    virtual void setPosition( const Pos& pos ) = 0;
    virtual void translate( const TranslationVector& moveVect ) = 0;

protected:
private:
    IMovable( const IMovable& value ) = delete;
    IMovable& operator=( const IMovable& value ) = delete;
};

NAMESPACE_END( LOGLW )