#include "Primitives/TriangleImpl.hpp"

using namespace LOGLW;

TriangleImpl::TriangleImpl()
{
}


void TriangleImpl::addShader( const IFile& shaderFile, IShaderFactory* sf )
{
    m_shaders.push_back( sf->createShader( shaderFile ) );
}

void TriangleImpl::render()
{
    applyShaders();
}

void TriangleImpl::setData( const TriangleData& triangleData )
{
    m_triangle = triangleData;
}

void TriangleImpl::applyShaders()
{
    for( auto& shader: m_shaders )
    {
        shader->useShader();
    }
}

// TODO
const Pos& TriangleImpl::getPos() const
{
    static Pos pos;
    return pos;
}

void TriangleImpl::setPosition( const Pos& )
{
}

void TriangleImpl::translate( const TranslationVector& )
{
}

TriangleImpl::~TriangleImpl()
{
}