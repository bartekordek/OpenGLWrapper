#pragma once

#include "libopenglwrapper/Program.hpp"
#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/VertexBuffer.hpp"
#include "libopenglwrapper/IndexBuffer.hpp"

#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/STL_IMPORTS/STD_cstdint.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

/*
A Vertex Array Object (VAO) is an OpenGL Object
that stores all of the state needed to supply
vertex data (with one minor exception noted below).
It stores the format of the vertex data as well as
the Buffer Objects (see below) providing the vertex data arrays.
Note that VAOs do not copy,
freeze or store the contents of the referenced buffers - if you
change any of the data in the buffers referenced by an existing VAO,
those changes will be seen by users of the VAO.

A Vertex Array Object (VAO) is an object which contains
one or more Vertex Buffer Objects and is designed to
store the information for a complete rendered object.
In our example this is a diamond consisting of four
vertices as well as a color for each vertex.
*/

// Vertex Array Objects (VAOs) are conceptually nothing but thin state wrappers.
// VAO - attribs?

NAMESPACE_BEGIN( LOGLW )

template<typename Type>
using Ptr = CUL::GUTILS::DumbPtr<Type>;

using BuffIDType = uint8_t;

class LIBOPENGLWRAPPER_API VertexArray final:
    public IUtilityUser,
    public IRenderable
{
public:
    explicit VertexArray();

    BuffIDType getId() const;
    void addVBO( VertexBuffer* vbo );

    VertexBuffer* const addVertexBuffer(std::vector<float>& vertices);
    IndexBuffer* const addIndexBuffer( std::vector<unsigned>& indices );

    void render() override;

    ~VertexArray();
protected:
private:
    void bind();
    void unbind();
    void release();

    unsigned m_bufferId = 0;
    std::vector<Ptr<Program>> m_shaderPrograms;
    std::vector<Ptr<VertexBuffer>> m_vbos;
    std::vector<unsigned> m_indices;
    std::vector<float> m_vertices;

    VertexArray( const VertexArray& value ) = delete;
    VertexArray( VertexArray&& value ) = delete;
    VertexArray& operator=( const VertexArray& value ) = delete;
    VertexArray& operator=( VertexArray&& value ) = delete;
};

NAMESPACE_END( LOGLW )