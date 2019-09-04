#include "ShaderConcrete.hpp"
#include "OpenGL_3_Utils.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"

NAMESPACE_BEGIN( LOGLW )

ShaderConcrete::ShaderConcrete( const IFile& shaderCode ):
    IShader( shaderCode ),
    m_id( OGLUTILS::createShader( shaderCode ) )
{
}

ShaderConcrete::~ShaderConcrete()
{
    if( m_id )
    {
        OGLUTILS::removeShader( m_id );
        m_id = 0;
    }
}

cunt ShaderConcrete::getId() const
{
    return m_id;
}

void ShaderConcrete::useShader() const
{
    OGLUTILS::useProgram( m_id );
}

NAMESPACE_END( LOGLW )