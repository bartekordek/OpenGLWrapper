#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

TriangleImpl::TriangleImpl()
{
}

void TriangleImpl::render()
{
    getUtility()->matrixStackPush();
    getUtility()->translate( getWorldPosition() );
    getUtility()->draw( m_values, m_colors );
    getUtility()->matrixStackPop();
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

void TriangleImpl::setValues( const ValuesArray& values )
{
    m_values = values;
    m_triangle.setValues( values );
}

void TriangleImpl::addShader(const CUL::FS::Path&, IShaderFactory* )
{

}

const std::vector<float> TriangleImpl::getData() const
{
    std::vector<float> result;
    return result;
}

void TriangleImpl::setColor( const TriangleColors& colors )
{
    m_colors = colors;
}

void TriangleImpl::setColor( const ColorS& colorInput )
{
    for( auto& color: m_colors )
    {
        color = colorInput;
    }
}

TriangleImpl::~TriangleImpl()
{
}