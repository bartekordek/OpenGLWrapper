#pragma once

#include "libopenglwrapper/ISprite.hpp"
#include "CUL/Graphics/IImage.hpp"

NAMESPACE_BEGIN( LOGLW )

class Sprite final:
    public ISprite,
    public IUtilityUser
{
public:
    Sprite();

    CUL::Graphics::IImage* m_image = nullptr;
    unsigned m_textureId = 0;

    void render() override;

    ~Sprite();
protected:
private:

// Deleted:
    Sprite( const Sprite& arg ) = delete;
    Sprite( Sprite&& arg ) = delete;
    Sprite& operator=( const Sprite& rhv ) = delete;
    Sprite& operator=( Sprite&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )