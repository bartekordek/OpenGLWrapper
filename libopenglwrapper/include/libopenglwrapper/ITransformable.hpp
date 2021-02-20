#pragma once

#include "libopenglwrapper/IRenderable.hpp"

#include "CUL/Graphics/IPosition3DD.hpp"
#include "CUL/Math/Point.hpp"
#include "CUL/Math/Angle.hpp"

#include "CUL/STL_IMPORTS/STD_array.hpp"

NAMESPACE_BEGIN( LOGLW )
class LIBOPENGLWRAPPER_API ITransformable
{
public:

    using Pos = CUL::MATH::Point;
    using TranslationVector = CUL::MATH::Point;

    ITransformable();

    void setWorldPosition( const Pos& position );
    void setWorldPosition( Pos::Type x, Pos::Type y, Pos::Type z );
    const Pos& getWorldPosition() const;

    void setWorldAngle( CUL::MATH::EulerAngles type, Pos::Type value );
    void setWorldAngle( CUL::MATH::EulerAngles type, const CUL::MATH::Angle& angle );

    float getWorldAngleF( CUL::MATH::EulerAngles type ) const;
    const CUL::MATH::Angle& getWorldAngle( CUL::MATH::EulerAngles type ) const;

    virtual ~ITransformable();

protected:
private:
    Pos m_worldPos;
    std::array<CUL::MATH::Angle, 3> m_angles;
// Deleted:
    ITransformable( const ITransformable& value ) = delete;
    ITransformable& operator=( const ITransformable& value ) = delete;
};

NAMESPACE_END( LOGLW )