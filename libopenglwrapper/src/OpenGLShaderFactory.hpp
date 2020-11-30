#pragma once

#include "libopenglwrapper/IShaderFactory.hpp"
#include "libopenglwrapper/IProgramFactory.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

using ShaderPtr = CUL::GUTILS::DumbPtr<IShader>;
using ProgramPtr = CUL::GUTILS::DumbPtr<IProgram>;

using MyString = CUL::String;
using ShaderMap = std::map<MyString, ShaderPtr>;
using ProgramMap = std::map<unsigned int, ProgramPtr>;

class IOpenGLWrapper;
class OpenGLShaderFactory final:
    public IShaderFactory,
    public IProgramFactory
{
public:
    explicit OpenGLShaderFactory( IOpenGLWrapper* wrapper );
    ~OpenGLShaderFactory();

    IShader* createShader( const CUL::FS::Path& filePath ) override;
    IProgram* createProgram() override;
    IShader* getShader( const CUL::FS::Path& filePath ) override;

protected:
private:
    OpenGLShaderFactory( const OpenGLShaderFactory& arg ) = delete;
    OpenGLShaderFactory& operator=( const OpenGLShaderFactory& rhv ) = delete;

    bool shaderExist( const CUL::FS::Path& filePath ) const;
    
    IShader* addShader( const CUL::FS::Path& filePath );

    mutable ShaderMap m_shaders;
    ProgramMap m_programs;
    IOpenGLWrapper& m_openglWrapper;
    CUL::CULInterface* m_culInterface = nullptr;
};

NAMESPACE_END( LOGLW )