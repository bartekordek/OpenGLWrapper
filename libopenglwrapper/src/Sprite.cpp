#include "libopenglwrapper/Sprite.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/VertexArray.hpp"
#include "libopenglwrapper/Program.hpp"
#include "libopenglwrapper/Shader.hpp"

#include "CUL/Graphics/IImageLoader.hpp"
#include "CUL/Math/Algorithms.hpp"
#include "CUL/Filesystem/FileFactory.hpp"
#include "CUL/Graphics/IImage.hpp"

#undef LoadImage

using namespace LOGLW;

Sprite::Sprite( CUL::CULInterface* cul ) : m_cul(cul)
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


void Sprite::init()
{
    m_shaderProgram = std::make_unique<Program>();
    m_shaderProgram->initialize();

    const std::string vertexShaderSource =
#include "embedded_shaders/camera.vert"
        ;

    const std::string fragmentShaderSource =
#include "embedded_shaders/camera.frag"
        ;

    auto fragmentShaderFile = m_cul->getFF()->createRegularFileRawPtr( "embedded_shaders/camera.frag" );
    fragmentShaderFile->loadFromString( fragmentShaderSource );
    auto fragmentShader = new Shader( fragmentShaderFile );

    auto vertexShaderCode = m_cul->getFF()->createRegularFileRawPtr( "embedded_shaders/camera.vert" );
    vertexShaderCode->loadFromString( vertexShaderSource );
    auto vertexShader = new Shader( vertexShaderCode );

    m_shaderProgram->attachShader( vertexShader );
    m_shaderProgram->attachShader( fragmentShader );
    m_shaderProgram->link();
    m_shaderProgram->validate();

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


    m_arrayBufferId = getUtility()->generateBuffer( LOGLW::BufferTypes::VERTEX_ARRAY );
    getUtility()->bindBuffer( BufferTypes::VERTEX_ARRAY, m_arrayBufferId );
    getUtility()->enableVertexAttribArray( 0 );
    getUtility()->enableVertexAttribArray( 1 );

    auto bufferId = getUtility()->generateBuffer( BufferTypes::ARRAY_BUFFER );
        

    std::vector<float> data = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f };

    getUtility()->bufferData( bufferId, data, BufferTypes::ARRAY_BUFFER );
    getUtility()->vertexAttribPointer( 0, 3, DataType::FLOAT, false, 5 * sizeof( float ) );
    getUtility()->vertexAttribPointer( 1, 2, DataType::FLOAT, false, 5 * sizeof( float ), (void*)( 3 * sizeof(float) ) );


    getUtility()->unbindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->unbindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );


    m_shaderProgram->enable();
    m_shaderProgram->setAttrib( "texture1", 0 );
    m_shaderProgram->disable();

    m_initialized = true;
}


void Sprite::renderModern()
{
    if( !m_initialized )
    {
        init();
    }

    getUtility()->setActiveTexture(0);
    getUtility()->bindTexture( m_textureId );

    m_shaderProgram->enable();

    glm::vec3 Position = glm::vec3( 0, 0, 0 );
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.f, -1.f, 0.f);
    glm::mat4 viewMatrix = glm::lookAt( Position, Position + Front, up );


    glm::mat4 projection = glm::perspective( glm::radians(90.f ), 16.f/9.f, 0.1f, 100.0f );

    glm::mat4 model = glm::mat4( 1.0f );  // make sure to initialize matrix to identity matrix first

    const Pos& position = getWorldPosition();
    glm::vec3 m_pos = position.toGlmVec();
    model = glm::translate( model, m_pos );

    m_shaderProgram->setAttrib( "projection", projection );
    m_shaderProgram->setAttrib( "view", viewMatrix );
    m_shaderProgram->setAttrib( "model", model );

    getUtility()->bindBuffer(BufferTypes::VERTEX_ARRAY, m_arrayBufferId);

    getUtility()->drawArrays(PrimitiveType::TRIANGLES, 0, 6);

    m_shaderProgram->disable();

    getUtility()->bindBuffer( BufferTypes::VERTEX_ARRAY, 0 );
    getUtility()->bindTexture( 0u );
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
