#pragma once

#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/IObject.hpp"

#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/Graphics/Pos3D.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API ITriangle:
    public IObject
{
public:
    ITriangle();

    virtual void render() = 0;

    virtual const Pos& getPos() const = 0;
    virtual void setPosition( const Pos& pos ) = 0;
    virtual void translate( const TranslationVector& moveVect ) = 0;

    virtual void setP1( const Point& val ) = 0;
    virtual void setP2( const Point& val ) = 0;
    virtual void setP3( const Point& val ) = 0;

    virtual const Point& getP1() const = 0;
    virtual const Point& getP2() const = 0;
    virtual const Point& getP3() const = 0;

    virtual void setValues( const ValuesArray& values ) = 0;
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