#pragma once

#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IndexBuffer final : private IUtilityUser
{
public:
    using DataType = std::vector<unsigned>;
private:
    unsigned m_id = 0;

    DataType m_data;

public:
    IndexBuffer();
    void loadData( DataType& data );
    const DataType& getData() const;
    ~IndexBuffer();

private:
    IndexBuffer( const IndexBuffer& arg ) = delete;
    IndexBuffer( IndexBuffer&& arg ) = delete;
    IndexBuffer& operator=( const IndexBuffer& rhv ) = delete;
    IndexBuffer& operator=( IndexBuffer&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )