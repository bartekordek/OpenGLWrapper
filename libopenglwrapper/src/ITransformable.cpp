#include "libopenglwrapper/ITransformable.hpp"

using namespace LOGLW;

ITransformable::ITransformable()
{

}

void ITransformable::setWorldPosition( const Pos& position )
{
    m_worldPos = position;
}

void ITransformable::setWorldPosition( Type x, Type y, Type z )
{
    m_worldPos.x = x;
    m_worldPos.y = y;
    m_worldPos.z = z;
}

const Pos& ITransformable::getWorldPosition() const
{
    return m_worldPos;
}

void ITransformable::setWorldAngle(CUL::MATH::EulerAngles type, Type value)
{
    m_angles[(size_t)type].setValue( value, CUL::MATH::Angle::Type::RADIAN );
}

void ITransformable::setWorldAngle( CUL::MATH::EulerAngles type, const CUL::MATH::Angle& angle )
{
    m_angles[ (size_t) type ] = angle;
}

float ITransformable::getWorldAngleF(CUL::MATH::EulerAngles type) const
{
    return m_angles[ (size_t) type].getValueF();
}

const CUL::MATH::Angle& ITransformable::getWorldAngle( CUL::MATH::EulerAngles type ) const
{
    return m_angles[ (size_t) type ];
}

ITransformable::~ITransformable()
{

}