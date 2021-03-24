#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API Model final:
    public IObject,
    private IUtilityUser
{
public:
    Model();

    virtual ~Model() = default;


    std::vector<float> m_vertices;
protected:
private:
    void render();

    // Deleted:
    Model( const Model& arg ) = delete;
    Model( Model&& arg ) = delete;
    Model& operator=( const Model& rhv ) = delete;
    Model& operator=( Model&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )