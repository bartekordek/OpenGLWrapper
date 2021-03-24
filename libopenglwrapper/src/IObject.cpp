#include "libopenglwrapper/IObject.hpp"

using namespace LOGLW;

IObject::IObject()
{
}

// Dummy
void IObject::addShader( const CUL::FS::Path&, IShaderFactory* )
{
}

// Dummy
const std::vector<float> IObject::getVertices() const
{
    return std::vector<float>();
}

IObject::~IObject()
{
}