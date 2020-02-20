#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

Triangle::Triangle():
        m_impl( new TriangleImpl )
{
}

void Triangle::addShader(
    IFile* file,
    IShaderFactory* sf )
{
    m_impl->addShader( file, sf );
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

Triangle::~Triangle()
{
    delete m_impl;
    m_impl = nullptr;
}