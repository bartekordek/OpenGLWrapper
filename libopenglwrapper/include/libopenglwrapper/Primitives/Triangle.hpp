#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "CUL/Math/Triangle.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/Graphics/Pos3D.hpp"

NAMESPACE_BEGIN( LOGLW )

using Triangle3DF = CUL::MATH::Triangle3DF;
using IFile = CUL::FS::IFile;
using Point = CUL::MATH::PointF;

class TriangleImpl;
class LIBOPENGLWRAPPER_API Triangle final:
    public IObject
{
public:
    Triangle();

    void addShader(
        IFile* shaderFile,
        IShaderFactory* sf ) override;
    void render();

    const Pos& getPos() const;
    void setPosition( const Pos& pos );
    void translate( const TranslationVector& moveVect );

    void setP1( const Point& val );
    void setP2( const Point& val );
    void setP3( const Point& val );

    const Point& getP1() const;
    const Point& getP2() const;
    const Point& getP3() const;

    const std::vector<float> getData() const override;

    ~Triangle();
protected:
private:
    Triangle( const Triangle& args ) = delete;
    Triangle( Triangle&& args ) = delete;
    Triangle& operator=( const Triangle& rhv ) = delete;
    Triangle& operator=( Triangle&& rhv ) = delete;

    TriangleImpl* m_impl = nullptr;
};

NAMESPACE_END( LOGLW )