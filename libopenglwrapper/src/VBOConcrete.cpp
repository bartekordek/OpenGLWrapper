#include "VBOConcrete.hpp"
#include "libopenglwrapper/IUtility.hpp"

using namespace LOGLW;

VBOConcrete::VBOConcrete()
{
}

void VBOConcrete::setId( Cunt id )
{
    m_bufferId = id;
}

unsigned int VBOConcrete::getId() const
{
    return m_bufferId;
}

void VBOConcrete::setData( const FloatData& data )
{
    m_data = data;
}

void VBOConcrete::setDataFromObject( IObject* object )
{
    m_data = std::move( object->getData() );
    getUtility()->bufferData( m_data, LOGLW::BufferTypes::ARRAY_BUFFER );

    const auto dataTypeSize = static_cast<int>( sizeof( float ) );
    const auto dataSize = static_cast<int>( m_data.size() ) / 3; // xyz = 3.

    getUtility()->vertexAttribPointer(
        0,
        3,
        LOGLW::DataType::FLOAT,
        false,
        dataSize * dataTypeSize

    );
    getUtility()->enableVertexAttribArray( 0 );
}

VBOConcrete::~VBOConcrete()
{
}