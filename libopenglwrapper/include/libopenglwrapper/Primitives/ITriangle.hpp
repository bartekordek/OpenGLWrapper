#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/Graphics/Pos3D.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API ITriangle:
    public IObject
{
public:
    ITriangle();

    virtual void setValues( const TriangleData& values ) = 0;
    virtual void setColor( const TriangleColors& colors ) = 0;
    virtual void setColor( const ColorS& color ) = 0;

    virtual ~ITriangle();
protected:
private:
    ITriangle( const ITriangle& args ) = delete;
    ITriangle( ITriangle&& args ) = delete;
    ITriangle& operator=( const ITriangle& rhv ) = delete;
    ITriangle& operator=( ITriangle&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )