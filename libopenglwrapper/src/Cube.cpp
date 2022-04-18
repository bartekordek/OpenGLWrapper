#include "libopenglwrapper/Cube.hpp"

using namespace LOGLW;


Cube::Cube(Camera* camera, CUL::CULInterface* cul): m_camera(camera), m_cul( cul )
{

}

//void Cube::setImage(unsigned wallIndex, const CUL::FS::Path& imagePath, CUL::Graphics::IImageLoader* imageLoader)
void Cube::setImage(unsigned, const CUL::FS::Path&, CUL::Graphics::IImageLoader*)
{

}

void Cube::render()
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

Cube::~Cube()
{

}