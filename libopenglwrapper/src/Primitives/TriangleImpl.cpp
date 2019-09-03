#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

TriangleImpl::TriangleImpl()
{
}

TriangleImpl::TriangleImpl( const TriangleImpl& args ):
    m_triangle( args.m_triangle ),
    m_shaders( args.m_shaders )
{
}

TriangleImpl& TriangleImpl::operator=( const TriangleImpl& rhv )
{
    if( this != &rhv )
    {
        m_triangle = rhv.m_triangle;
    }
    return *this;
}

void TriangleImpl::addShader( const IFile& shaderFile, IShaderFactory* sf )
{
    m_shaders.push_back( sf->createShader( shaderFile ) );
}

void TriangleImpl::render()
{
}

const Pos& TriangleImpl::getPos() const
{
    static Pos pos;
    return pos;
}

void TriangleImpl::setPosition( const Pos& )
{
}

void TriangleImpl::translate( const TranslationVector& )
{
}

TriangleImpl::~TriangleImpl()
{
}