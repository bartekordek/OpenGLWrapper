#include "libopenglwrapper/Primitives/IRect.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"

using namespace LOGLW;

// TODO
//void IRect::addShader( const IFile& shaderFile, IShaderFactory* sf )
//{
//    CUL::Assert::simple(
//        shaderFile.exists(),
//        "File: " + shaderFile.getPath().getPath() + " does not exist." );
//    CUL::Assert::simple( sf != nullptr, "Shader factory is null." );
//
//    m_shader = sf->createShader( shaderFile );
//}