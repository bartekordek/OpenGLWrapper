#pragma once

#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

#include "CUL/STL_IMPORTS/STD_cstdint.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_atomic.hpp"

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

class LIBOPENGLWRAPPER_API VertexBuffer final : public IUtilityUser,
                                                public IRenderable
{
public:
    VertexBuffer( std::vector<float>& data, bool instantLoad = true );
    void render() override;
    void bufferData();
    unsigned getId()const;
    int getSize() const;
    ~VertexBuffer();

protected:
private:
    void loadData( std::vector<float>& data );
    void release();

    unsigned m_bufferId = 0;
    std::vector<float> m_vertices;
    std::atomic<bool> m_load = true;

    VertexBuffer( const VertexBuffer& value ) = delete;
    VertexBuffer( VertexBuffer&& value ) = delete;
    VertexBuffer& operator=( const VertexBuffer& value ) = delete;
    VertexBuffer& operator=( VertexBuffer&& value ) = delete;
};

NAMESPACE_END( LOGLW )