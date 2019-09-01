#include "OpenGLShaderFactory.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
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
    if( shaderExist( shaderCode ) )
    {
        return getShader( shaderCode );
    }
    else
    {
        return addShader( shaderCode );
    }
}

const bool OpenGLShaderFactory::shaderExist( const IFile & shaderCode ) const
{
    ShaderMap::iterator it = m_shaders.find( shaderCode.getPath().getPath() );
    return m_shaders.end() == it;
}

IShader* OpenGLShaderFactory::getShader( const IFile& shaderCode )
{
    return m_shaders[ shaderCode.getPath().getPath() ].get();
}

IShader* OpenGLShaderFactory::addShader( const IFile& shaderCode )
{
    auto shader = new ShaderConcrete( shaderCode );
    m_shaders[ shaderCode.getPath().getPath() ] = shader;
    return shader;
}