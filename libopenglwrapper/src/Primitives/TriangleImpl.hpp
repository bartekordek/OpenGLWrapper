#pragma once

#include "libopenglwrapper/Primitives/ITriangle.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

class TriangleImpl final:
    public ITriangle
{
public:
    TriangleImpl();
    TriangleImpl( const TriangleImpl& args );

    TriangleImpl& operator=( const TriangleImpl& rhv );

    void addShader( const IFile& shaderFile, IShaderFactory* sf ) override;
    void render();

    const Pos& getPos() const override;
    void setPosition( const Pos& pos ) override;
    void translate( const TranslationVector& moveVect ) override;

    ~TriangleImpl();

protected:
private:
    Triangle3DF m_triangle;
    std::vector<IShader*> m_shaders;

};

NAMESPACE_END( LOGLW )