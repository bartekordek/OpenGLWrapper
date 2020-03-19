#pragma once

#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IUtilityUser
{
public:
    IUtilityUser();

    static void useUtility( IUtility* utility );

    virtual ~IUtilityUser();
protected:
    static IUtility* getUtility();

private:
    IUtilityUser( const IUtilityUser& arg ) = delete;
    IUtilityUser( IUtilityUser&& arg ) = delete;
    IUtilityUser& operator=( const IUtilityUser& arg ) = delete;
    IUtilityUser& operator=( IUtilityUser&& arg ) = delete;

    static IUtility* s_utility;

};

NAMESPACE_END( LOGLW )