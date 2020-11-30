#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

Triangle::Triangle():
        m_impl( new TriangleImpl )
{
}

void Triangle::addShader(
    const CUL::FS::Path& filePath,
    IShaderFactory* sf )
{
    m_impl->addShader( filePath, sf );
}

void Triangle::render()
{
    m_impl->render();
}

const Pos& Triangle::getPos() const
{
    return m_impl->getPos();
}

void Triangle::setPosition( const Pos& pos )
{
    m_impl->setPosition( pos );
}

void Triangle::translate( const TranslationVector& moveVect )
{
    m_impl->translate( moveVect );
}

void Triangle::setP1( const Point& val )
{
    m_impl->setP1( val );
}

void Triangle::setP2( const Point& val )
{
    m_impl->setP2( val );
}

void Triangle::setP3( const Point& val )
{
    m_impl->setP3( val );
}

const Point& Triangle::getP1() const
{
    return m_impl->getP1();
}

const Point& Triangle::getP2() const
{
    return m_impl->getP2();
}

const Point& Triangle::getP3() const
{
    return m_impl->getP3();
}

const std::vector<float> Triangle::getData() const
{
    std::vector<float> result;
    result.resize( 9 );

    result[ 0 ] = m_impl->getP1().getX();
    result[ 1 ] = m_impl->getP1().getY();
    result[ 2 ] = m_impl->getP1().getZ();

    result[ 3 ] = m_impl->getP2().getX();
    result[ 4 ] = m_impl->getP2().getY();
    result[ 5 ] = m_impl->getP2().getZ();

    result[ 6 ] = m_impl->getP3().getX();
    result[ 7 ] = m_impl->getP3().getY();
    result[ 8 ] = m_impl->getP3().getZ();

    return result;
}

Triangle::~Triangle()
{
    delete m_impl;
    m_impl = nullptr;
}