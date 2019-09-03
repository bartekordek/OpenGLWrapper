#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

Triangle::Triangle():
    m_impl( new TriangleImpl() )
{
}

Triangle::Triangle( const Triangle& args ):
    m_impl( new TriangleImpl( *args.m_impl ) )
{
}

Triangle& Triangle::operator=( const Triangle& rhv )
{
    if( this != &rhv )
    {
        *m_impl = *rhv.m_impl;
    }
    return *this;
}

void Triangle::addShader( const IFile& shaderFile, IShaderFactory* sf )
{
    m_impl->addShader( shaderFile, sf );
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

Triangle::~Triangle()
{
}