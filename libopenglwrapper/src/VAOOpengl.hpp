#pragma once

#include "libopenglwrapper/IVAO.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"

NAMESPACE_BEGIN( LOGLW )

class VAOOpengl final:
    public IVAO
{
public:
    VAOOpengl();

    void setId( Cunt id ) override;

    ~VAOOpengl();
protected:
private:
    unsigned int getId() const override;
    void addVBO( IVBO* vbo ) override;

    unsigned m_id = 0;
    std::map<Cunt, IVBO*> m_vbos;

    VAOOpengl( const VAOOpengl& value ) = delete;
    VAOOpengl( VAOOpengl&& value ) = delete;
    VAOOpengl& operator=( const VAOOpengl& value ) = delete;
    VAOOpengl& operator=( VAOOpengl&& value ) = delete;
};

NAMESPACE_END( LOGLW )