#pragma once

#include "libopenglwrapper/IObject.hpp"

#include "CUL/STL_IMPORTS/STD_array.hpp"

NAMESPACE_BEGIN( CUL )
class CULInterface;
NAMESPACE_END( CUL )

NAMESPACE_BEGIN( LOGLW )

class Camera;

class LIBOPENGLWRAPPER_API Cube final: public IObject, public IUtilityUser
{
public:
    Cube( Camera* camera, CUL::CULInterface* cul );

    void setImage(unsigned wallIndex, const CUL::FS::Path& imagePath, CUL::Graphics::IImageLoader* imageLoader);

    ~Cube();
protected:
private:
    void render() override;
    void renderModern();
    void renderLegacy();
    Camera* m_camera = nullptr;
    CUL::CULInterface* m_cul = nullptr;

    // Deleted:
    Cube( const Cube& arg ) = delete;
    Cube( Cube&& arg ) = delete;
    Cube& operator=( const Cube& rhv ) = delete;
    Cube& operator=( Cube&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )