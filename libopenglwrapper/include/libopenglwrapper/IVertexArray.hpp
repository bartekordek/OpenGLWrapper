#pragma once

#include "libopenglwrapper/IUtilityUser.hpp"

#include "CUL/STL_IMPORTS/STD_cstdint.hpp"

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

class IVertexBuffer;

using BuffIDType = uint8_t;

class LIBOPENGLWRAPPER_API IVertexArray:
    public IUtilityUser
{
public:
    explicit IVertexArray();

    virtual void setId( BuffIDType id ) = 0;
    virtual BuffIDType getId() const = 0;
    virtual void addVBO( IVertexBuffer* vbo ) = 0;

    virtual ~IVertexArray();
protected:
private:


    IVertexArray( const IVertexArray& value ) = delete;
    IVertexArray( IVertexArray&& value ) = delete;
    IVertexArray& operator=( const IVertexArray& value ) = delete;
    IVertexArray& operator=( IVertexArray&& value ) = delete;
};

NAMESPACE_END( LOGLW )