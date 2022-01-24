#include "libopenglwrapper/Sprite.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/VertexArray.hpp"

#include "IMPORT_glew.hpp"
#include "ImportFreeglut.hpp"

#include "CUL/Graphics/IImageLoader.hpp"

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
                        unsigned textureId )
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

    getUtility()->bindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER, m_arrayBufferId );
    getUtility()->bindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER, m_elementBufferId );

    std::vector<TextureData2D> vData( 4 );

    auto powerOfTwo = []( unsigned num )
    {
        if( num != 0 )
        {
            num--;
            num |= ( num >> 1 );   // Or first 2 bits
            num |= ( num >> 2 );   // Or next 2 bits
            num |= ( num >> 4 );   // Or next 4 bits
            num |= ( num >> 8 );   // Or next 8 bits
            num |= ( num >> 16 );  // Or next 16 bits
            num++;
        }
        return num;
    };

    auto imgSize = m_image->getImageInfo().size;
    auto canvasSize = m_image->getImageInfo().canvasSize;

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

    // Vertex positions
    vData[0].x = 0.f;
    vData[0].y = quadHeight;

    vData[1].x = quadWidth;
    vData[1].y = quadHeight;

    vData[2].x = quadWidth;
    vData[2].y = 0.f;

    vData[3].x = 0.f;
    vData[3].y = 0.f;

    glBindTexture( GL_TEXTURE_2D, m_textureId );

    // Enable vertex and texture coordinate arrays
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, m_arrayBufferId );
    // Update vertex buffer data
    size_t dataSize = 4 * sizeof( TextureData2D );
    glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, vData.data() );

    // Set texture coordinate data
    const int coordDataSize = 2;
    GLenum dataType = GL_FLOAT;
    GLsizei stride = sizeof( TextureData2D );
    auto texCoordOffset = offsetof( TextureData2D, s );
    const void* pointerCoord = (GLvoid*)texCoordOffset;
    glTexCoordPointer( coordDataSize, dataType, stride, pointerCoord );

    // Set vertex data
    auto positionOffset = offsetof( TextureData2D, x );
    auto pointerPosition = (GLvoid*)positionOffset;
    glVertexPointer( coordDataSize, dataType, stride, pointerPosition );

    // Draw quad using vertex data and index data
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId );
    glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

    // Disable vertex and texture coordinate arrays
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    getUtility()->unbindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->unbindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );
}

void Sprite::init()
{
    m_textureId = getUtility()->generateTexture();
    getUtility()->bindTexture( m_textureId );

    const auto& ii = getImageInfo();
    TextureInfo td;
    td.pixelFormat = CUL::Graphics::PixelFormat::RGBA;
    td.size = ii.canvasSize;
    td.data = getData();
    td.textureId = m_textureId;

    getUtility()->setTextureData( td );

    getUtility()->setTextureParameter( TextureParameters::MAG_FILTER, TextureFilterType::LINEAR );
    getUtility()->setTextureParameter( TextureParameters::MIN_FILTER, TextureFilterType::LINEAR );

    {
        auto buffType = LOGLW::BufferTypes::ARRAY_BUFFER;
        std::vector<TextureData2D> data( 4 );
        m_arrayBufferId = getUtility()->generateBuffer( buffType );
        getUtility()->bindBuffer( buffType, m_arrayBufferId );
        getUtility()->bufferData( data, buffType );
    }

    {
        auto buffType = LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER;
        m_elementBufferId = getUtility()->generateBuffer( buffType );
        getUtility()->bindBuffer( buffType, m_elementBufferId );
        std::vector<unsigned> iData( 4 );
        iData[0] = 0;
        iData[1] = 1;
        iData[2] = 2;
        iData[3] = 3;
        getUtility()->bufferData( iData, buffType );
    }

    getUtility()->unbindBuffer( LOGLW::BufferTypes::ARRAY_BUFFER );
    getUtility()->unbindBuffer( LOGLW::BufferTypes::ELEMENT_ARRAY_BUFFER );

    m_initialized = true;
}

void Sprite::registerTask( TaskType taskType )
{
    std::lock_guard<std::mutex> guard( m_tasksMtx );
    if( taskType == TaskType::CREATE_VAO )
    {
        if( taskIsAlreadyPlaced( taskType ) )
        {
            return;
        }
    }
    m_tasks.push_back( taskType );
}

void Sprite::runTasks()
{
    std::lock_guard<std::mutex> tasksGuard( m_tasksMtx );
    while( !m_tasks.empty() )
    {
        auto task = m_tasks.front();

        if( task == TaskType::CREATE_VAO )
        {
            m_vao = new VertexArray( false );

            VertexBufferData vboData;
            vboData.vertices = {
                // positions          // colors           // texture coords
                0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
                0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
                -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
            };

            vboData.indices = {
                0, 1, 3,  // first triangle
                1, 2, 3   // second triangle
            };

            vboData.primitiveType = PrimitiveType::QUADS;
            vboData.containsColorData = true;
            vboData.containsTextureCoords = true;

            m_vao->addVertexBuffer( vboData, true );

            m_textureId = getUtility()->generateTexture();
            getUtility()->bindTexture( m_textureId );
            getUtility()->setClientState( ClientStateTypes::VERTEX_ARRAY, true );
            getUtility()->setClientState( ClientStateTypes::TEXTURE_COORD_ARRAY, true );

            m_vao->getVertexBuffer()->bind();

            auto& ii = m_image->getImageInfo();

            TextureInfo ti;
            ti.border = 0;
            ti.data = m_image->getData();
            ti.dataType = DataType::UNSIGNED_SHORT;
            ti.pixelFormat = CUL::Graphics::PixelFormat::RGBA;
            ti.size = ii.size;
            ti.textureId = 2137;
            getUtility()->setTextureData( ti );

            /*getUtility()->setTextureParameter( TextureParameters::WRAP_S, TextureFilterType:: );*/
            // glBindTexture(GL_TEXTURE_2D, texture1);
            // GLuint

            /*m_vao0->addVertexBuffer( VBO_Data );
            m_vao0->createShader( "vertexShader.vert" );
            m_vao0->createShader( "fragmentShader.frag" );*/
        }

        m_tasks.pop_front();
    }
}

bool Sprite::taskIsAlreadyPlaced( TaskType tt ) const
{
    return std::find( m_tasks.begin(), m_tasks.end(), tt ) != m_tasks.end();
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