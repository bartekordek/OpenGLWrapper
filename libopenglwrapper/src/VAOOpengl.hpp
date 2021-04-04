#pragma once

#include "libopenglwrapper/IVertexArray.hpp"
#include "CUL/STL_IMPORTS/STD_map.hpp"

NAMESPACE_BEGIN( LOGLW )

class VAOOpengl final:
    public IVertexArray
{
public:
    VAOOpengl();

    void setId( BuffIDType id ) override;

    ~VAOOpengl();
protected:
private:
    BuffIDType getId() const override;
    void addVBO( IVertexBuffer* vbo ) override;

    BuffIDType m_id = 0;
    std::map<BuffIDType, IVertexBuffer*> m_vbos;

    VAOOpengl( const VAOOpengl& value ) = delete;
    VAOOpengl( VAOOpengl&& value ) = delete;
    VAOOpengl& operator=( const VAOOpengl& value ) = delete;
    VAOOpengl& operator=( VAOOpengl&& value ) = delete;
};

NAMESPACE_END( LOGLW )