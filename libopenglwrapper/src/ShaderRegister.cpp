#include "ShaderRegister.hpp"

using namespace LOGLW;

ShaderRegister::ShaderRegister()
{
}

IShader* ShaderRegister::getShader( const Path& )
{
    return nullptr;
}

ShaderRegister::~ShaderRegister()
{
}