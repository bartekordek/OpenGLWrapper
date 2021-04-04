#pragma once

#include "libopenglwrapper/IVertexArray.hpp"
#include "libopenglwrapper/IVertexBuffer.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IBufferFactory
{
public:
    IBufferFactory() = default;

    virtual IVertexBuffer* createVBO() = 0;
    virtual IVertexArray* createVAO() = 0;

    ~IBufferFactory() = default;
protected:
private:
    IBufferFactory( const IBufferFactory& arg ) = delete;
    IBufferFactory( IBufferFactory&& arg ) = delete;
    IBufferFactory& operator=( const IBufferFactory& rhv ) = delete;
    IBufferFactory& operator=( IBufferFactory&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )