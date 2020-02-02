#pragma once

#include "libopenglwrapper/Import.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

using Cfloat = const float;

using FloatData = std::vector<float>;

class LIBOPENGLWRAPPER_API IVBO
{
public:
    IVBO() = default;
    virtual ~IVBO() = default;

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