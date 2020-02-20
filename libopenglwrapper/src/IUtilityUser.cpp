#include "libopenglwrapper/IUtilityUser.hpp"

using namespace LOGLW;

IUtilityUser::IUtilityUser()
{
}

void IUtilityUser::useUtility( IUtility* utility )
{
    m_utility = utility;
}

IUtility* IUtilityUser::getUtility() const
{
    return m_utility;
}


IUtilityUser::~IUtilityUser()
{
}