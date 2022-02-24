#include "libopenglwrapper/Sprite.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/VertexArray.hpp"

#include "CUL/Graphics/IImageLoader.hpp"
#include "CUL/Math/Algorithms.hpp"
#include "CUL/Graphics/IImage.hpp"

#undef LoadImage

using namespace LOGLW;

Sprite::Sprite()
{
}

void Sprite::LoadImage( const CUL::FS::Path& imagePath, CUL::Graphics::IImageLoader* imageLoader )
{
    m_image = imageLoader->loadImage( imagePath );
}

void Sprite::LoadImage( CUL::Graphics::DataType* data, unsigned width, unsigned height, CUL::Graphics::IImageLoader* imageLoader,
                        unsigned )
{
    m_image = imageLoader->loadImage( (unsigned char*)data, width, height );
}

void Sprite::render()
{
    if( getUtility()->isLegacy() )
    {
        renderLegacy();
    }
    else
    {
        renderModern();
    }
}

const CUL::Graphics::ImageInfo& Sprite::getImageInfo() const
{
    return m_image->getImageInfo();
}

CUL::Graphics::DataType* Sprite::getData() const
{
    return m_image->getData();
}

void Sprite::renderModern()
{
    if( !m_initialized )
    {
        init();
    }

    ITransformable::Pos worldPosition =  ITransformable::getWorldPosition();

    auto imgSize = m_image->getImageInfo().size;
    auto canvasSize = m_image->getImageInfo().canvasSize;

    auto pivotTimesSize = ITransformable::getPivot();
    pivotTimesSize.x() *= imgSize.width;
    pivotTimesSize.y() *= imgSize.height;

    ITransformable::Pos targetPosition = worldPosition - pivotTimesSize;

    getUtility()->translate(targetPosition);
    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_arrayBufferId );
    getUtility()->bindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER, m_elementBufferId );

    std::vector<TextureData2D> vData( 4 );

    float texTop = 0.f;
    float texBottom = (float)imgSize.height / (float)canvasSize.height;
    float texLeft = 0.f;
    float texRight = (float)imgSize.width / (float)canvasSize.width;

    QuadSimple tex;
    tex.bottom = (float)imgSize.height / canvasSize.height;
    tex.topRight = (float)imgSize.width / canvasSize.width;

    auto quadWidth = static_cast<float>( imgSize.width );
    auto quadHeight = static_cast<float>( imgSize.height );

    vData[0].s = texLeft;
    vData[0].t = texTop;
    vData[1].s = texRight;
    vData[1].t = texTop;
    vData[2].s = texRight;
    vData[2].t = texBottom;
    vData[3].s = texLeft;
    vData[3].t = texBottom;

    vData[0].x = 0.f;
    vData[0].y = quadHeight;

    vData[1].x = quadWidth;
    vData[1].y = quadHeight;

    vData[2].x = quadWidth;
    vData[2].y = 0.f;

    vData[3].x = 0.f;
    vData[3].y = 0.f;

    getUtility()->bindTexture( m_textureId );

    getUtility()->setClientState( ClientStateTypes::VERTEX_ARRAY, true );
    getUtility()->setClientState( ClientStateTypes::TEXTURE_COORD_ARRAY, true );

    getUtility()->bufferSubdata( m_arrayBufferId, BufferTypes::ARRAY_BUFFER, vData );

    auto texCoordOffset = offsetof( TextureData2D, s );
    getUtility()->texCoordPointer( 2, DataType::FLOAT, sizeof( TextureData2D ), (void*)texCoordOffset );

    auto positionOffset = offsetof( TextureData2D, x );
    const size_t stride = sizeof( TextureData2D );
    getUtility()->vertexPointer( 2, DataType::FLOAT, stride, (void*)positionOffset );

    getUtility()->bindBuffer( BufferTypes::ELEMENT_ARRAY_BUFFER, m_elementBufferId );

    getUtility()->drawElementsFromLastBuffer(PrimitiveType::QUADS, DataType::UNSIGNED_INT,4);

    getUtility()->setClientState( ClientStateTypes::TEXTURE_COORD_ARRAY, false );
    getUtility()->setClientState( ClientStateTypes::VERTEX_ARRAY, false );

    getUtility()->unbindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->unbindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );
}

void Sprite::init()
{
    m_textureId = getUtility()->generateTexture();

    const auto& ii = getImageInfo();
    TextureInfo td;
    td.pixelFormat = CUL::Graphics::PixelFormat::RGBA;
    td.size = ii.canvasSize;
    td.data = getData();
    td.textureId = m_textureId;

    getUtility()->setTextureData( m_textureId, td );

    getUtility()->setTextureParameter( m_textureId, TextureParameters::MAG_FILTER, TextureFilterType::LINEAR );
    getUtility()->setTextureParameter( m_textureId, TextureParameters::MIN_FILTER, TextureFilterType::LINEAR );

    {
        auto buffType = LOGLW::BufferTypes::ARRAY_BUFFER;
        std::vector<TextureData2D> data( 4 );
        m_arrayBufferId = getUtility()->generateBuffer( buffType );
        getUtility()->bufferData( m_arrayBufferId, data, buffType );
    }

    {
        auto buffType = LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER;
        m_elementBufferId = getUtility()->generateBuffer( buffType );
        std::vector<unsigned> iData( 4 );
        iData[0] = 0;
        iData[1] = 1;
        iData[2] = 2;
        iData[3] = 3;
        getUtility()->bufferData( m_elementBufferId, iData, buffType );
    }

    getUtility()->unbindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->unbindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );

    m_initialized = true;
}

void Sprite::renderLegacy()
{
    Quad quad1;

    std::array<std::array<float, 3>, 4> values;
    values[3] = { 0.f, 0.f, 0.f };
    values[2] = { 1.f, 0.f, 0.f };
    values[1] = { 1.f, 1.f, 0.f };
    values[0] = { 0.f, 1.f, 0.f };
    quad1.setData( values );

    Quad quad2;
    const auto& size = m_image->getImageInfo().size;
    values[0] = {
        0.f,
        0.f,
        0.f,
    };
    values[1] = {
        (float)size.width,
        0.f,
        0.f,
    };
    values[2] = {
        (float)size.width,
        (float)size.height,
        0.f,
    };
    values[3] = { 0.f, (float)size.height, 0.f };

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
