#include "ShaderConcrete.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

ShaderConcrete::ShaderConcrete( IFile* shaderCode, IUtility* util ):
    m_utility( util ),
    m_shaderCode( shaderCode ),
    m_filePath( shaderCode->getPath() )
{
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

const String& ShaderConcrete::getPath() const
{
    return m_filePath;
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