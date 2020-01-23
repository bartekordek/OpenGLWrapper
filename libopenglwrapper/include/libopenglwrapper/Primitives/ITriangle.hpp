#pragma once

#include "libopenglwrapper/IObject.hpp"
#include "CUL/Math/Triangle.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"

NAMESPACE_BEGIN( LOGLW )

using Triangle3DF = CUL::Math::Triangle3DF;
using IFile = CUL::FS::IFile;

class LIBOPENGLWRAPPER_API ITriangle:
    public IObject
{
public:
    ITriangle() = default;

    virtual void addShader(
        const IFile& shaderFile,
        IShaderFactory* sf ) = 0;
    virtual void render() = 0;

    virtual const Pos& getPos() const = 0;
    virtual void setPosition( const Pos& pos ) = 0;
    virtual void translate( const TranslationVector& moveVect ) = 0;

    virtual ~ITriangle() = default;

protected:
private:
    ITriangle( const ITriangle& args ) = delete;
    ITriangle( ITriangle&& args ) = delete;
    ITriangle& operator=( const ITriangle& rhv ) = delete;
    ITriangle& operator=( ITriangle&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )