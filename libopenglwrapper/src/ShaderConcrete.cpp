#include "ShaderConcrete.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "CUL/Filesystem/FileFactory.hpp"

NAMESPACE_BEGIN( LOGLW )

ShaderConcrete::ShaderConcrete( IFile* filePath, IUtility* util ):
    m_utility( util )
{
    m_shaderCode = filePath;
    m_shaderCode->load( true );
    createShader();
}

Cunt ShaderConcrete::getId() const
{
    return m_id;
}

void ShaderConcrete::useShader() const
{
    m_utility->useProgram( m_id );
}

void ShaderConcrete::reload()
{
    removeShader();
    createShader();
}

const Path& ShaderConcrete::getPath() const
{
    auto shaderCode = m_shaderCode.get();
    return shaderCode->getPath();
}

void ShaderConcrete::createShader()
{
    m_id = m_utility->createShader( *m_shaderCode );
}

void ShaderConcrete::removeShader()
{
    if( m_id )
    {
        m_utility->removeShader( m_id );
        m_id = 0;
    }
}

ShaderConcrete::~ShaderConcrete()
{
    removeShader();
}


NAMESPACE_END( LOGLW )