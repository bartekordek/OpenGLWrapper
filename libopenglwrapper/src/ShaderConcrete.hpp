#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

NAMESPACE_BEGIN( LOGLW )

class ShaderConcrete final:
    public IShader
{
public:
    ShaderConcrete( IFile* file, IUtility* util );
    ~ShaderConcrete();

    unsigned int getId() const override;
    void useShader() const override;
    void reload() override;
    const Path& getPath() const override;

protected:
private:
    void createShader();
    void removeShader();

    ShaderConcrete() = delete;
    ShaderConcrete( const ShaderConcrete& arg ) = delete;
    ShaderConcrete& operator=( const ShaderConcrete& arg ) = delete;


    IUtility* m_utility = nullptr;
    unsigned int m_id = 0;
    CUL::GUTILS::DumbPtr<IFile> m_shaderCode;
};

NAMESPACE_END( LOGLW )