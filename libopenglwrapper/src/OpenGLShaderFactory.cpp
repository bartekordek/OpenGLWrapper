#include "OpenGLShaderFactory.hpp"
#include "CUL/SimpleAssert.hpp"
#include "ShaderConcrete.hpp"

using OpenGLShaderFactory = LOGLW::OpenGLShaderFactory;
using ShaderPtr = LOGLW::ShaderPtr;
using IFile = LOGLW::IFile;
using IShader = LOGLW::IShader;


OpenGLShaderFactory::OpenGLShaderFactory()
{
}


OpenGLShaderFactory::~OpenGLShaderFactory()
{
}

IShader* OpenGLShaderFactory::createShader( const IFile& shaderCode )
{
    auto shader = new ShaderConcrete( shaderCode );
    return shader;
}