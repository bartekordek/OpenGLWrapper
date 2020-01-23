#include "libopenglwrapper/Import.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IVertexBuffer
{
public:
    IVertexBuffer() = default;
    virtual ~IVertexBuffer() = default;

    virtual void bindData(
        const void* data,
        const size_t length ) = 0;

protected:
private:
    IVertexBuffer( const IVertexBuffer& arg ) = delete;
    IVertexBuffer& operator=( const IVertexBuffer& rhv ) = delete;
};

NAMESPACE_END( LOGLW )