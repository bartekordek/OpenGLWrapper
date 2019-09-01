#include "libopenglwrapper/Primitives/Rect.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/GenericUtils/SimpleAssert.hpp"

using namespace LOGLW;

Rect::Rect()
{
}

Rect::Rect( const Rect& arg ):
    m_rectDimension( arg.m_rectDimension ),
    m_shader( arg.m_shader )
{
}

Rect::Rect( const RectDimension& rectDimension ):
    m_rectDimension( rectDimension )
{
}

Rect& Rect::operator=( const Rect& rhv )
{
    if( this != &rhv )
    {
        m_rectDimension = rhv.m_rectDimension;
        m_shader = rhv.m_shader;
    }
    return *this;
}

void Rect::addShader( const IFile& shaderFile, IShaderFactory* sf )
{
    CUL::Assert::simple(
        shaderFile.exists(),
        "File: " + shaderFile.getPath().getPath() + " does not exist." );
    CUL::Assert::simple( sf != nullptr, "Shader factory is null." );

    m_shader = sf->createShader( shaderFile );
}

const Size& Rect::getSize() const
{
    return m_rectDimension.size;
}

const Pos& Rect::getPos() const
{
    return m_rectDimension.pos;
}

Rect::~Rect()
{
}