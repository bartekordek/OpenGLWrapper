#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "OpenGLShaderFactory.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "ShaderConcrete.hpp"

using OpenGLShaderFactory = LOGLW::OpenGLShaderFactory;
using ShaderPtr = LOGLW::ShaderPtr;
using IFile = LOGLW::IFile;
using IShader = LOGLW::IShader;

auto logger = CUL::LOG::LOG_CONTAINER::getLogger();

OpenGLShaderFactory::OpenGLShaderFactory()
{
}

OpenGLShaderFactory::~OpenGLShaderFactory()
{
}

IShader* OpenGLShaderFactory::createShader( const IFile& shaderCode )
{
    logger->log( "OpenGLShaderFactory::createShader: " + shaderCode.getPath() );
    if( shaderExist( shaderCode ) )
    {
        logger->log( "OpenGLShaderFactory::createShader: shader exist.");
        return getShader( shaderCode );
    }
    else
    {
        logger->log( "OpenGLShaderFactory::createShader: shader does not exist." );
        return addShader( shaderCode );
    }
}

const bool OpenGLShaderFactory::shaderExist( const IFile & shaderCode ) const
{
    ShaderMap::iterator it = m_shaders.find( shaderCode.getPath().getPath() );
    return m_shaders.end() != it;
}

IShader* OpenGLShaderFactory::getShader( const IFile& shaderCode )
{
    return m_shaders[ shaderCode.getPath().getPath() ].get();
}

IShader* OpenGLShaderFactory::addShader( const IFile& shaderCode )
{
    logger->log( "OpenGLShaderFactory::addShader: creating: " + shaderCode.getPath() );
    auto shader = new ShaderConcrete( shaderCode );
    m_shaders[ shaderCode.getPath().getPath() ] = shader;
    return shader;
}