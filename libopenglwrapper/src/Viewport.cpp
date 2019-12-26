#include "libopenglwrapper/Viewport.hpp"
#include "CUL/STL_IMPORTS/STD_utility.hpp"

using namespace LOGLW;

Viewport::Viewport()
{
}

Viewport::Viewport( const Viewport& val ):
    pos( val.pos ),
    size(val.size )
{
}

Viewport::Viewport( Viewport&& val ):
    pos( std::move( val.pos ) ),
    size( std::move( val.size ) )
{
}

Cfloat Viewport::getAspectRatio() const
{
    return size.getHeight() * 1.0f / size.getHeight() * 1.0f;
}

Viewport& Viewport::operator=( const Viewport& rhv )
{
    if( this != & rhv )
    {
        pos = rhv.pos;
        size = rhv.size;
    }
    return *this;
}

Viewport& Viewport::operator=( Viewport&& rhv )
{
    if( this != &rhv )
    {
        pos = std::move( rhv.pos );
        size = rhv.size;
    }
    return *this;
}

Viewport::~Viewport()
{
}