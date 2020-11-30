#pragma once

#include "libopenglwrapper/IShader.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

#include "CUL/GenericUtils/DumbPtr.hpp"


NAMESPACE_BEGIN( CUL )
NAMESPACE_BEGIN( FS )
class Path;
class IFile;
NAMESPACE_END( FS )
NAMESPACE_END( CUL )

NAMESPACE_BEGIN( LOGLW )

class ShaderConcrete final:
    public IShader
{
public:
    ShaderConcrete( CUL::FS::IFile* file, IUtility* util );
    ~ShaderConcrete();

    unsigned int getId() const override;
    void useShader() const override;
    void reload() override;
    const CUL::FS::Path& getPath() const override;

protected:
private:
    void createShader();
    void removeShader();

    ShaderConcrete() = delete;
    ShaderConcrete( const ShaderConcrete& arg ) = delete;
    ShaderConcrete& operator=( const ShaderConcrete& arg ) = delete;


    IUtility* m_utility = nullptr;
    unsigned int m_id = 0;
    CUL::GUTILS::DumbPtr<CUL::FS::IFile> m_shaderCode;
};

NAMESPACE_END( LOGLW )