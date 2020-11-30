#include "OpenGLShaderFactory.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/Log/ILogContainer.hpp"
#include "ShaderConcrete.hpp"
#include "ProgramConcrete.hpp"
#include "libopenglwrapper/IOpenGLWrapper.hpp"

using OpenGLShaderFactory = LOGLW::OpenGLShaderFactory;
using ShaderPtr = LOGLW::ShaderPtr;
using IFile = LOGLW::IFile;
using IShader = LOGLW::IShader;
using IProgram = LOGLW::IProgram;

auto logger = CUL::LOG::LOG_CONTAINER::getLogger();

OpenGLShaderFactory::OpenGLShaderFactory( LOGLW::IOpenGLWrapper* wrapper ):
    m_openglWrapper( *wrapper ),
    m_culInterface( wrapper->getCul() )
{
}

IShader* OpenGLShaderFactory::createShader( const CUL::FS::Path& filePath )
{
    auto result = getShader( filePath );
    if( result )
    {
        return result;
    }
    else
    {
        return addShader( filePath );
    }
}

IProgram* OpenGLShaderFactory::createProgram()
{
    logger->log( "OpenGLShaderFactory::createProgram()" );

    IProgram* result = new ProgramConcrete( IUtilityUser::getUtility(), *this );
    m_programs[result->getProgramId()] = result;
    return result;
}

bool OpenGLShaderFactory::shaderExist( const CUL::FS::Path& filePath ) const
{
    ShaderMap::iterator it = m_shaders.find( filePath );
    return m_shaders.end() != it;
}

IShader* OpenGLShaderFactory::getShader( const CUL::FS::Path& filePath )
{
    return m_shaders[ filePath ].get();
}

IShader* OpenGLShaderFactory::addShader( const CUL::FS::Path& filePath )
{
    logger->log( "OpenGLShaderFactory::addShader: creating: " + filePath );
    auto shaderPtr = m_culInterface->getFF()->createFileFromPath( filePath );
    auto shader = new ShaderConcrete( shaderPtr, IUtilityUser::getUtility() );
    m_shaders[ filePath ] = shader;
    return shader;
}

OpenGLShaderFactory::~OpenGLShaderFactory()
{
    m_shaders.clear();
    m_programs.clear();
}
