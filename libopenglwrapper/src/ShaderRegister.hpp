#pragma once

#include "libopenglwrapper/IShader.hpp"

#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )
class ShaderRegister
{
public:
    ShaderRegister();

    //const
    IShader* getShader( const Path& filePath );

    ~ShaderRegister();

protected:
private:
    ShaderRegister( const ShaderRegister& arg ) = delete;
    ShaderRegister& operator=( const ShaderRegister& rhv ) = delete;

    std::map<CUL::String, CUL::GUTILS::DumbPtr<IShader>> m_shaders;
};

NAMESPACE_END( LOGLW )
