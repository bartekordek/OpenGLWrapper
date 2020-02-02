#pragma once

#include "libopenglwrapper/Import.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IVAO
{
public:
    IVAO() = default;
    virtual ~IVAO() = default;

protected:
private:
    IVAO( const IVAO& value ) = delete;
    IVAO( IVAO&& value ) = delete;
    IVAO& operator=( const IVAO& value ) = delete;
    IVAO& operator=( IVAO&& value ) = delete;
};

NAMESPACE_END( LOGLW )