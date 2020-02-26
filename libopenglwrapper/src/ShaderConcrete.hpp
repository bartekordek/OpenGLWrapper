#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

NAMESPACE_BEGIN( LOGLW )

class ShaderConcrete final:
    public IShader
{
public:
    ShaderConcrete( IFile* shaderCode, IUtility* util );
    ~ShaderConcrete();

    Cunt getId() const override;
    void useShader() const override;
    void reload() override;
    const String& getPath() const override;

protected:
private:
    void createShader();
    void removeShader();

    ShaderConcrete() = delete;
    ShaderConcrete( const ShaderConcrete& arg ) = delete;
    ShaderConcrete& operator=( const ShaderConcrete& arg ) = delete;

    IUtility* m_utility = nullptr;
    unsigned int m_id = 0;
    IFile* m_shaderCode = nullptr;
    String m_filePath;
};

NAMESPACE_END( LOGLW )