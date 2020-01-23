#pragma once

#include "libopenglwrapper/Primitives/ITriangle.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"

NAMESPACE_BEGIN( LOGLW )

using TriangleData = Triangle3DF;

class TriangleImpl final:
    public ITriangle
{
public:
    TriangleImpl();

    void addShader( const IFile& shaderFile, IShaderFactory* sf ) override;
    void render() override;

    void setData( const TriangleData& triangleData );

    ~TriangleImpl();

protected:
private:
    const Pos& getPos() const override;
    void setPosition( const Pos& pos ) override;
    void translate( const TranslationVector& moveVect ) override;

    void applyShaders();

    TriangleData m_triangle;
    std::vector<IShader*> m_shaders;

private: // Deleted:
    TriangleImpl( const TriangleImpl& args ) = delete;
    TriangleImpl( TriangleImpl&& args ) = delete;
    TriangleImpl& operator=( const TriangleImpl& rhv ) = delete;
    TriangleImpl& operator=( TriangleImpl&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )