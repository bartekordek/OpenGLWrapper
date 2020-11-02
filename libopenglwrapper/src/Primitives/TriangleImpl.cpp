#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

TriangleImpl::TriangleImpl()
{
}

void TriangleImpl::addShader( const Path& filePath, IShaderFactory* sf )
{
    m_shaders.push_back( sf->createShader( filePath ) );
}

void TriangleImpl::render()
{
    applyShaders();
}

void TriangleImpl::applyShaders()
{
    for( auto& shader: m_shaders )
    {
        shader->useShader();
    }
}

// TODO
const Point& TriangleImpl::getPos() const
{
    return m_triangle.getCentralPosition();
}

// TODO:
void TriangleImpl::setPosition( const Pos& )
{
}

void TriangleImpl::translate( const TranslationVector& tv )
{
    m_triangle += tv;
}

void TriangleImpl::setP1( const Point& val )
{
    m_triangle.setP1( val );
}

void TriangleImpl::setP2( const Point& val )
{
    m_triangle.setP2( val );
}

void TriangleImpl::setP3( const Point& val )
{
    m_triangle.setP3( val );
}

const Point& TriangleImpl::getP1() const
{
    return m_triangle.getP1();
}

const Point& TriangleImpl::getP2() const
{
    return m_triangle.getP2();
}

const Point& TriangleImpl::getP3() const
{
    return m_triangle.getP3();
}

TriangleImpl::~TriangleImpl()
{
}