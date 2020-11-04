#pragma once

#include "libopenglwrapper/Import.hpp"
#include "CUL/Graphics/Pos2D.hpp"
#include "CUL/Graphics/Size2D.hpp"

NAMESPACE_BEGIN( LOGLW )

using Pos2Di = CUL::Graphics::Pos2Di;
using Size2Di = CUL::Graphics::Size2Di;

class LIBOPENGLWRAPPER_API Viewport final
{
public:
    Pos2Di pos;
    Size2Di size;

protected:
private:
};

NAMESPACE_END( LOGLW )