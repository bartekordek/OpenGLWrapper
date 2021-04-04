#include "libopenglwrapper/IVertexArray.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API VAOConcrete final:
    public IUtilityUser
{
public:
    VAOConcrete();

    ~VAOConcrete();
protected:
private:
    VAOConcrete( const VAOConcrete& arg ) = delete;
    VAOConcrete( VAOConcrete&& arg ) = delete;
    VAOConcrete& operator=( const VAOConcrete& arg ) = delete;
    VAOConcrete& operator=( VAOConcrete&& arg ) = delete;

};

NAMESPACE_END( LOGLW )