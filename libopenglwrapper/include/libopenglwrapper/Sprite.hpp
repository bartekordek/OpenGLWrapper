#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/VertexArray.hpp"

#include "CUL/Graphics/IImage.hpp"

#include "CUL/STL_IMPORTS/STD_deque.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API Sprite final: public IObject, public IUtilityUser
{
public:
    Sprite();

    void LoadImage( const CUL::FS::Path& imagePath, CUL::Graphics::IImageLoader* imageLoader );
    void LoadImage( CUL::Graphics::DataType* data, unsigned width, unsigned height, CUL::Graphics::IImageLoader*, unsigned textureId );

    unsigned m_textureId = 0u;

    void render() override;
    const CUL::Graphics::ImageInfo& getImageInfo() const;
    CUL::Graphics::DataType* getData() const;

    ~Sprite();

protected:
private:
    void init();
    bool m_initialized = false;
    enum class TaskType : short
    {
        CREATE_VAO = 1,
        CREATE_VBO,
        CREATE_VAO_VBO
    };

    void renderModern();
    void renderLegacy();
    CUL::Graphics::IImage* m_image = nullptr;

    CUL::GUTILS::DumbPtr<VertexArray> m_vao;

    unsigned m_arrayBufferId = 0u;
    unsigned m_elementBufferId = 0u;

    // Deleted:
    Sprite( const Sprite& arg ) = delete;
    Sprite( Sprite&& arg ) = delete;
    Sprite& operator=( const Sprite& rhv ) = delete;
    Sprite& operator=( Sprite&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )