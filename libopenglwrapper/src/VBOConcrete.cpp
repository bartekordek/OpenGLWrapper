#include "VBOConcrete.hpp"

using namespace LOGLW;

VBOConcrete::VBOConcrete()
{
}

void VBOConcrete::setId( Cunt id )
{
    m_bufferId = id;
}

Cunt VBOConcrete::getId() const
{
    return m_bufferId;
}

void VBOConcrete::setData( const FloatData& data )
{
    m_data = data;
}

VBOConcrete::~VBOConcrete()
{
}