#pragma once

#include "libopenglwrapper/Primitives/ITriangle.hpp"
#include "libopenglwrapper/IShader.hpp"
#include "libopenglwrapper/IUtilityUser.hpp"
#include "libopenglwrapper/IUtility.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/Math/Primitives/Triangle3D.hpp"

NAMESPACE_BEGIN( LOGLW )

class TriangleImpl final: public ITriangle, public IUtilityUser
{
public:
    TriangleImpl();

    void render() override;

    void setP1( const Point& val ) override;
    void setP2( const Point& val ) override;
    void setP3( const Point& val ) override;

    const Point& getP1() const override;
    const Point& getP2() const override;
    const Point& getP3() const override;

    ~TriangleImpl();

protected:
    void setValues( const ValuesArray& values ) override;
    void addShader( const CUL::FS::Path& filePath, IShaderFactory* sf ) override;
    const std::vector<float> getData() const override;
    void setColor( const TriangleColors& colors ) override;
    void setColor( const ColorS& color ) override;

private:
    ValuesArray m_values;
    CUL::MATH::Primitives::Triangle3DF m_triangle;
    std::array<ColorS, 3> m_colors;

// Deleted:
    TriangleImpl( const TriangleImpl& args ) = delete;
    TriangleImpl( TriangleImpl&& args ) = delete;
    TriangleImpl& operator=( const TriangleImpl& rhv ) = delete;
    TriangleImpl& operator=( TriangleImpl&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )