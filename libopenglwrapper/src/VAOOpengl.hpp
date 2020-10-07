#pragma once

#include "libopenglwrapper/IVAO.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"

NAMESPACE_BEGIN( LOGLW )

class VAOOpengl final:
    public IVAO
{
public:
    VAOOpengl();

    ~VAOOpengl();
protected:
private:

    void setId( Cunt id ) override;
    Cunt getId() const override;
    void addVBO( IVBO* vbo ) override;
    IVBO* createVBO() override;

    unsigned m_id = 0;
    std::map<Cunt, IVBO*> m_vbos;

    VAOOpengl( const VAOOpengl& value ) = delete;
    VAOOpengl( VAOOpengl&& value ) = delete;
    VAOOpengl& operator=( const VAOOpengl& value ) = delete;
    VAOOpengl& operator=( VAOOpengl&& value ) = delete;
};

NAMESPACE_END( LOGLW )