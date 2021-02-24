#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API ILine:
    public IObject
{
public:
    ILine() = default;

    virtual void setValues( const LineData& values ) = 0;
    virtual void setColor( const LineColors& colors ) = 0;
    virtual void setColor( const ColorS& color ) = 0;

    virtual ~ILine() = default;

protected:
private:
    ILine( const ILine& arg ) = delete;
    ILine( ILine&& arg ) = delete;
    ILine& operator=( const ILine& rhv ) = delete;
    ILine& operator=( ILine&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )