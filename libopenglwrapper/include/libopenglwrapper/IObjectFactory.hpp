#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/Primitives/ITriangle.hpp"
#include "libopenglwrapper/Primitives/IQuad.hpp"

#include "CUL/Filesystem/IFile.hpp"

NAMESPACE_BEGIN( LOGLW )

using String = CUL::String;

class ISprite;

class LIBOPENGLWRAPPER_API IObjectFactory
{
public:
    IObjectFactory() = default;
    virtual ~IObjectFactory() = default;

    virtual IObject* createFromFile( IFile* file ) = 0;
    virtual IObject* createFromFile( const String& path ) = 0;

    virtual ITriangle* createTriangle( const TriangleData& data, const ColorS& color = ColorE::WHITE ) = 0;

    virtual IQuad* createQuad( const QuadData& data, const ColorS& color = ColorE::WHITE ) = 0;

    virtual ISprite* createSprite( const String& path ) = 0;

protected:
private:

// Deleted:
    IObjectFactory( const IObjectFactory& arg ) = delete;
    IObjectFactory( IObjectFactory&& arg ) = delete;
    IObjectFactory& operator=( const IObjectFactory& rhv ) = delete;
    IObjectFactory& operator=( IObjectFactory&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )