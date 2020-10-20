#include "ShaderRegister.hpp"

using namespace LOGLW;

ShaderRegister::ShaderRegister()
{
}

IShader* ShaderRegister::getShader( const IFile& )
{
    return nullptr;
}

ShaderRegister::~ShaderRegister()
{
}