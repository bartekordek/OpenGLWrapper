#include "libopenglwrapper/IVBO.hpp"
#include "VBOConcrete.hpp"

using namespace LOGLW;

IVBO* IVBO::createVBO()
{
    return new VBOConcrete();
}