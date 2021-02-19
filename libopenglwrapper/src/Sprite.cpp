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
    getUtility()->draw( quad2, quad1 );
}

Sprite::~Sprite()
{
}