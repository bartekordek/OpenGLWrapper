#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "CUL/Math/Triangle.hpp"

NAMESPACE_BEGIN( LOGLW )

using Triangle3DF = CUL::Math::Triangle3DF;

template <typename Type>
using DumbPtr = CUL::GUTILS::DumbPtr<Type>;

class TriangleImpl;

class LIBOPENGLWRAPPER_API Triangle:
    public IObject
{
public:
    Triangle();
    Triangle( const Triangle& args );

    Triangle& operator=( const Triangle& rhv );

    void addShader( const IFile& shaderFile, IShaderFactory* sf ) override;
    void render() override;

    const Pos& getPos() const override;
    void setPosition( const Pos& pos ) override;
    void translate( const TranslationVector& moveVect ) override;

    virtual ~Triangle();

protected:
private:
    DumbPtr<TriangleImpl> m_impl;
    //TriangleImpl* m_impl = nullptr;
};

NAMESPACE_END( LOGLW )