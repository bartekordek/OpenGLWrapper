#pragma once

#include "libopenglwrapper/IShaderFactory.hpp"


NAMESPACE_BEGIN( LOGLW )

class OpenGLShaderFactory final:
    public IShaderFactory
{
public:
    OpenGLShaderFactory();
    ~OpenGLShaderFactory();

    // Inherited via IShaderFactory
    virtual IShader* createShader( const IFile& shaderCode ) override;
protected:
private:

};

NAMESPACE_END( LOGLW )