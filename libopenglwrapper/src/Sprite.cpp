#include "Sprite.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "CUL/Graphics/IImage.hpp"

using namespace LOGLW;

Sprite::Sprite()
{
}

void Sprite::render()
{
    QuadF quad1;

    std::array<std::array<float, 3>,4 > values;
    values[ 3 ] = { 0.f, 0.f, 0.f };
    values[ 2 ] = { 1.f, 0.f, 0.f };
    values[ 1 ] = { 1.f, 1.f, 0.f };
    values[ 0 ] = { 0.f, 1.f, 0.f };
    quad1.setData( values );

    QuadF quad2;
    const auto& size = m_image->getImageInfo().size;
    values[ 0 ] = {               0.f,                 0.f, 0.f, };
    values[ 1 ] = { (float)size.width,                 0.f, 0.f, };
    values[ 2 ] = { (float)size.width,  (float)size.height, 0.f, };
    values[ 3 ] = {               0.f,  (float)size.height, 0.f };

    quad2.setData( values );

    getUtility()->bindTexture( m_textureId );

    getUtility()->matrixStackPush();
    getUtility()->translate( getWorldPosition() );
    static const auto type = CUL::MATH::Angle::Type::DEGREE;
    getUtility()->rotate( getWorldAngle( CUL::MATH::EulerAngles::YAW ).getValueF( type ), 0.f, 0.f, 1.f );
    getUtility()->rotate( getWorldAngle( CUL::MATH::EulerAngles::PITCH ).getValueF( type ), 0.f, 1.f, 0.f );
    getUtility()->rotate( getWorldAngle( CUL::MATH::EulerAngles::ROLL ).getValueF( type ), 1.f, 0.f, 0.f );
    getUtility()->draw( quad2, quad1 );
    getUtility()->matrixStackPop();

    getUtility()->bindTexture( 0 );
}

Sprite::~Sprite()
{
}