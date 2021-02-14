#include "libopenglwrapper/IObject.hpp"

using namespace LOGLW;

IObject::IObject()
{
}

void IObject::setWorldPosition( const Point& position )
{
    m_worldPos = position;
}

const Point& IObject::getWorldPosition()
{
    return m_worldPos;
}

IObject::~IObject()
{
}