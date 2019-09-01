#pragma once

#include "libopenglwrapper/IShaderFactory.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"


NAMESPACE_BEGIN( LOGLW )

using MyString = CUL::MyString;
using ShaderMap = std::map<MyString, ShaderPtr>;

class OpenGLShaderFactory final:
    public IShaderFactory
{
public:
    OpenGLShaderFactory();
    ~OpenGLShaderFactory();

    virtual IShader* createShader( const IFile& shaderCode ) override;

protected:
private:
    const bool shaderExist( const IFile& shaderCode ) const;
    IShader* getShader( const IFile& shaderCode );
    IShader* addShader( const IFile& shaderCode );

    mutable ShaderMap m_shaders;

};

NAMESPACE_END( LOGLW )