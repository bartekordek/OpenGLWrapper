#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IMovable.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/Filesystem/IFile.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

using Point = CUL::MATH::PointF;
class LIBOPENGLWRAPPER_API IObject:
    public IRenderable,
    public IMovable
{
public:
    IObject();

    virtual void addShader( const CUL::FS::Path& filePath, IShaderFactory* sf ) = 0;

    virtual const std::vector<float> getData() const = 0;

    void setWorldPosition( const Point& position );
    const Point& getWorldPosition();

    virtual ~IObject();
protected:
private:
    Point m_worldPos;

// Deleted:
    IObject( const IObject& value ) = delete;
    IObject( IObject&& value ) = delete;
    IObject& operator=( const IObject& value ) = delete;
    IObject& operator=( IObject&& value ) = delete;
};

NAMESPACE_END( LOGLW )