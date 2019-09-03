#pragma once

#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

class TriangleImpl final
{
public:
    TriangleImpl();
    TriangleImpl( const TriangleImpl& args );

    TriangleImpl& operator=( const TriangleImpl& rhv );

    void addShader( const IFile& shaderFile, IShaderFactory* sf );
    void render();

    const Pos& getPos() const;
    void setPosition( const Pos& pos );
    void translate( const TranslationVector& moveVect );

    ~TriangleImpl();

protected:
private:
    Triangle3DF m_triangle;
    std::vector<IShader*> m_shaders;

};

NAMESPACE_END( LOGLW )