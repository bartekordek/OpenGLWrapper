#include "libopenglwrapper/IVBO.hpp"

NAMESPACE_BEGIN( LOGLW )

class VBOConcrete final:
    public IVBO
{
public:
    VBOConcrete();

    void setId( Cunt id ) override;
    Cunt getId() const override;
    void setData( const FloatData& data ) override;

    ~VBOConcrete();
protected:
private:
    FloatData m_data;
    Uint m_bufferId = 0;

    // Deleted:
    VBOConcrete( const VBOConcrete& value ) = delete;
    VBOConcrete( VBOConcrete&& value ) = delete;
    VBOConcrete& operator=( const VBOConcrete& value ) = delete;
    VBOConcrete& operator=( VBOConcrete&& value ) = delete;
};

NAMESPACE_END( LOGLW )