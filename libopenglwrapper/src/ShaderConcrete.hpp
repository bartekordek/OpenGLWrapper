#pragma once

#include "libopenglwrapper/IShader.hpp"

NAMESPACE_BEGIN( LOGLW )

class ShaderConcrete final:
    public IShader
{
public:
    ShaderConcrete( const IFile& shaderCode );
    ~ShaderConcrete();

    cunt getId() const override;

protected:
private:
    ShaderConcrete() = delete;
    ShaderConcrete( const ShaderConcrete& arg ) = delete;
    ShaderConcrete& operator=( const ShaderConcrete& arg ) = delete;

    unsigned int m_id = 0;

};

NAMESPACE_END( LOGLW )