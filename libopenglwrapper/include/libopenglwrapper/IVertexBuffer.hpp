#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
#include "libopenglwrapper/IObject.hpp"

#include "CUL/STL_IMPORTS/STD_cstdint.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

/*
A Vertex Buffer Object (VBO) is the common term for
a normal Buffer Object when it is used as a
source for vertex array data.
It is no different from any other buffer object,
and a buffer object used for Transform Feedback
or asynchronous pixel transfers can be used as
source values for vertex arrays.
*/

/*
A Vertex Buffer Object (VBO) is a memory buffer
in the high speed memory of your video card designed
to hold information about vertices.
In our example we have two VBOs, one that
describes the coordinates of our vertices and
another that describes the color
associated with each vertex.
VBOs can also store information such as normals,
texcoords, indicies, etc.
*/

// Vertex Buffer Objects (VBOs) store actual data.

// Just data?

NAMESPACE_BEGIN( LOGLW )

using FloatData = std::vector<float>;
using BuffIDType = uint8_t;

class LIBOPENGLWRAPPER_API IVertexBuffer:
    public IUtilityUser
{
public:
    IVertexBuffer() = default;

    virtual void setId( BuffIDType id ) = 0;
    virtual BuffIDType getId() const = 0;

    virtual void setData( const FloatData& data ) = 0;
    virtual void setDataFromObject( IObject* object ) = 0;

    virtual ~IVertexBuffer() = default;
protected:
private:
    IVertexBuffer( const IVertexBuffer& value ) = delete;
    IVertexBuffer( IVertexBuffer&& value ) = delete;
    IVertexBuffer& operator=( const IVertexBuffer& value ) = delete;
    IVertexBuffer& operator=( IVertexBuffer&& value ) = delete;
};

NAMESPACE_END( LOGLW )