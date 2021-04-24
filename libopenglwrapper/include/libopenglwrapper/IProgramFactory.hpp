#pragma once

#include "libopenglwrapper/IUtilityUser.hpp"
#include "libopenglwrapper/Program.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IProgramFactory
{
public:
    IProgramFactory() = default;
    virtual ~IProgramFactory() = default;

    virtual Program* createProgram() = 0;

protected:
private:
    IProgramFactory( const IProgramFactory& arg ) = delete;
    IProgramFactory( IProgramFactory&& arg ) = delete;
    IProgramFactory& operator=( const IProgramFactory& rhv ) = delete;
    IProgramFactory& operator=( IProgramFactory&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )