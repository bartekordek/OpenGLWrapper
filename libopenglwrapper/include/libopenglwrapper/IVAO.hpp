#pragma once

#include "libopenglwrapper/IVBO.hpp"

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

*/

/*
A Vertex Array Object (VAO) is an object which contains
one or more Vertex Buffer Objects and is designed to
store the information for a complete rendered object.
In our example this is a diamond consisting of four
vertices as well as a color for each vertex.
*/

// Vertex Array Objects (VAOs) are conceptually nothing but thin state wrappers.

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IVAO
{
public:
    IVAO() = default;
    virtual ~IVAO() = default;

    virtual void addVBO( IVBO* vbo ) = 0;

    static IVAO* createVAO();

protected:
private:
    IVAO( const IVAO& value ) = delete;
    IVAO( IVAO&& value ) = delete;
    IVAO& operator=( const IVAO& value ) = delete;
    IVAO& operator=( IVAO&& value ) = delete;
};

NAMESPACE_END( LOGLW )