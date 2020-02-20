#pragma once

#include "CUL/UselessMacros.hpp"

#if _WIN32
#define LIBOPENGLWRAPPER_WINDOWS
#define LIBOPENGLWRAPPER_EXPORT
#else
#define LIBOPENGLWRAPPER_LINUX
#define LIBOPENGLWRAPPER_EXPORT
#endif

#if defined LIBOPENGLWRAPPER_EXPORT && defined LIBOPENGLWRAPPER_LINUX
#define LIBOPENGLWRAPPER_API
#define LIBOPENGLWRAPPER_POST
#define LIBOPENGLWRAPPER_TEMPLATE
#elif defined LIBOPENGLWRAPPER_EXPORT && defined LIBOPENGLWRAPPER_WINDOWS
#define LIBOPENGLWRAPPER_API __declspec(dllexport)
#define LIBOPENGLWRAPPER_POST __cdecl
#define LIBOPENGLWRAPPER_TEMPLATE
#else
#define LIBOPENGLWRAPPER __declspec(dllimport)
#define LIBOPENGLWRAPPER_POST __cdecl
#define LIBOPENGLWRAPPER_TEMPLATE extern
#endif

#if _MSC_VER
#if defined(_M_X64) || defined(__amd64__)
#pragma warning( disable: 4820 )
#endif
#endif

using Cunt = const unsigned int;
using Cbool = const bool;