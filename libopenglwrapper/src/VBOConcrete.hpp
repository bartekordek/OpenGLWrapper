#include "libopenglwrapper/IVBO.hpp"

NAMESPACE_BEGIN( LOGLW )

class VBOConcrete final: public IVBO
{
public:
    VBOConcrete();

    void setData( const FloatData& data ) override;

    ~VBOConcrete();
protected:
private:
    FloatData m_data;
    // Delted:
    VBOConcrete( const VBOConcrete& value ) = delete;
    VBOConcrete( VBOConcrete&& value ) = delete;
    VBOConcrete& operator=( const VBOConcrete& value ) = delete;
    VBOConcrete& operator=( VBOConcrete&& value ) = delete;
};

NAMESPACE_END( LOGLW )