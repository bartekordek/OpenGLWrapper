#include "libopenglwrapper/IVAO.hpp"
#include "VAOConcrete.hpp"
#include "VAOOpengl.hpp"
#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

IVAO* IVAO::createVAO()
{
    IVAO* vao = new VAOOpengl();
    const auto vaoId = getUtility()->generateAndBindBuffer( LOGLW::BufferTypes::VERTEX_ARRAY );
    vao->setId( vaoId );
    return vao;
}

IVAO::IVAO()
{
}

IVAO::~IVAO()
{

}