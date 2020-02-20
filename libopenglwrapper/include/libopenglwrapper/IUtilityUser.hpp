#pragma once

#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IUtilityUser
{
public:
    IUtilityUser();

    void useUtility( IUtility* utility );

    virtual ~IUtilityUser();
protected:
    IUtility* getUtility() const;

private:
    IUtilityUser( const IUtilityUser& arg ) = delete;
    IUtilityUser( IUtilityUser&& arg ) = delete;
    IUtilityUser& operator=( const IUtilityUser& arg ) = delete;
    IUtilityUser& operator=( IUtilityUser&& arg ) = delete;

    IUtility* m_utility = nullptr;

};

NAMESPACE_END( LOGLW )