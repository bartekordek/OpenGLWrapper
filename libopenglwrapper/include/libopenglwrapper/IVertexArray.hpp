#include "libopenglwrapper/IVertexBuffer.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API IVertexArray
{
public:
    IVertexArray() = default;
    virtual ~IVertexArray() = default;

protected:
private:
    IVertexArray( const IVertexArray& arg ) = delete;
    IVertexArray& operator=( const IVertexArray& rhv ) = delete;
};

NAMESPACE_END( LOGLW )