#pragma once

#include "libopenglwrapper/Import.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
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

class LIBOPENGLWRAPPER_API IVBO:
    public IUtilityUser
{
public:
    IVBO() = default;
    virtual ~IVBO() = default;

    virtual void setId( Cunt id ) = 0;
    virtual Cunt getId() const = 0;

    virtual void setData( const FloatData& data ) = 0;

    static IVBO* createVBO();

protected:
private:
    IVBO( const IVBO& value ) = delete;
    IVBO( IVBO&& value ) = delete;
    IVBO& operator=( const IVBO& value ) = delete;
    IVBO& operator=( IVBO&& value ) = delete;
};

NAMESPACE_END( LOGLW )