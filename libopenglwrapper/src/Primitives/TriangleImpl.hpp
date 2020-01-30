#pragma once

#include "libopenglwrapper/Primitives/Triangle.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/Math/Triangle.hpp"

NAMESPACE_BEGIN( LOGLW )

class TriangleImpl final
{
public:
    TriangleImpl();

    void addShader( const IFile& shaderFile, IShaderFactory* sf );
    void render();

    const Point& getPos() const;
    void setPosition( const Pos& pos );
    void translate( const TranslationVector& moveVect );

    void setP1( const Point& val );
    void setP2( const Point& val );
    void setP3( const Point& val );

    const Point& getP1() const;
    const Point& getP2() const;
    const Point& getP3() const;

    void applyShaders();


    ~TriangleImpl();

protected:
private:
    CUL::MATH::Triangle3DF m_triangle;
    std::vector<IShader*> m_shaders;

private: // Deleted:
    TriangleImpl( const TriangleImpl& args ) = delete;
    TriangleImpl( TriangleImpl&& args ) = delete;
    TriangleImpl& operator=( const TriangleImpl& rhv ) = delete;
    TriangleImpl& operator=( TriangleImpl&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )