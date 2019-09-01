#pragma once

#include "libopenglwrapper/IObject.hpp"

#include "CUL/Graphics/SimpleSize3D.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using Size = CUL::Graphics::SimpleSize3D<float>;

struct RectDimension
{
    Size size;
    Pos pos;

    RectDimension(){}

    RectDimension( const RectDimension& arg ):
        size( arg.size ),
        pos( arg.pos )
    {

    }

    RectDimension& operator=( const RectDimension& rhv )
    {
        if( this != & rhv )
        {
            size = rhv.size;
            pos = rhv.pos;
        }
        return *this;
    }

};

class LIBOPENGLWRAPPER_API Rect:
    public IObject
{
public:
    Rect();
    Rect( const Rect& arg );
    Rect( const RectDimension& rectDimension );

    Rect& operator=( const Rect& rhv );

    void addShader( const IFile& shaderFile, IShaderFactory* sf ) override;

    const Size& getSize() const;
    const Pos& getPos() const override;

    virtual ~Rect();

protected:
private:
    RectDimension m_rectDimension;
    IShader* m_shader = nullptr;

};

NAMESPACE_END( LOGLW )