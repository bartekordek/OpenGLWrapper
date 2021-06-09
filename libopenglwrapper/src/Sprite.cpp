#include "libopenglwrapper/Sprite.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/VertexArray.hpp"

#include "CUL/Graphics/IImage.hpp"

using namespace LOGLW;

Sprite::Sprite()
{
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

void Sprite::renderModern()
{
    runTasks();

    if( m_vao.get() == nullptr )
    {
        registerTask( TaskType::CREATE_VAO );
    }
    else
    {
        // TODO:
    }
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
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
            };

            vboData.indices =
            {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
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
            ti.textureId =2137;
            getUtility()->setTextureData( ti );

            /*getUtility()->setTextureParameter( TextureParameters::WRAP_S, TextureFilterType:: );*/
            //glBindTexture(GL_TEXTURE_2D, texture1);
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

    std::array<std::array<float, 3>,4 > values;
    values[ 3 ] = { 0.f, 0.f, 0.f };
    values[ 2 ] = { 1.f, 0.f, 0.f };
    values[ 1 ] = { 1.f, 1.f, 0.f };
    values[ 0 ] = { 0.f, 1.f, 0.f };
    quad1.setData( values );

    Quad quad2;
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