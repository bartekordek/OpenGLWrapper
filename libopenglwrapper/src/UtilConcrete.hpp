#pragma once

#include "libopenglwrapper/IUtility.hpp"

NAMESPACE_BEGIN( LOGLW )

class UtilConcrete final:
    public IUtility
{
public:
    UtilConcrete();

    void setViewPort( const Viewport& rect ) const override;
    void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar ) const override;
    void setOrthogonalPerspective( const Viewport& vp ) const override;
    void setPerspectiveProjection( const Viewport& vp ) const override;
    void lookAt( const Viewport& vp ) const override;
    void lookAt( const std::array< Pos3Dd, 3>& lookAtVec ) const override;
    void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const override;

    const ShaderTypes getShaderType( CUL::CsStr& fileExtension ) const override;

    Cunt createProgram() const override;
    void removeProgram( Cunt programId ) const override;
    void useProgram( Cunt programId ) const override;
    void linkProgram( Cunt programId ) const override;
    void validateProgram( Cunt programId ) const override;

    Cunt createShader( const IFile& shaderCode ) const override;
    void attachShader( Cunt programId, Cunt shaderId ) const override;
    void dettachShader( Cunt programId, Cunt shaderId ) const override;
    void removeShader( Cunt shaderId ) const override;

    String initContextVersion( Cunt major, Cunt minor ) const override;
    void setAttribValue( Cint attributeLocation, Cfloat value ) const override;
    void setAttribValue( Cint attributeLocation, Cint value ) const override;
    void setAttribValue( Cint attributeLocation, Cunt value ) const override;

    void setProjectionAndModelToIdentity() const override;
    void clearColorAndDepthBuffer() const override; 
    void createQuad( Cfloat scale = 1.0f ) const override;
    void clearColorTo( const ColorS color ) const override;
    void clearBuffer( const ClearMasks mask ) const override;

    Cunt generateVertexArray (const int size = 1 ) const override;

    Cunt generateArrayBuffer( const int size = 1 ) const override;
    void bufferData( const std::vector<unsigned int>& data, const BufferTypes type ) const override;
    void bufferData( const std::vector<float>& data, const BufferTypes type ) const override;
    void bufferData( const float vertices[] ) const override;

    Cunt generateAndBindBuffer( const BufferTypes bufferType, const int size = 1 ) const override;
    Cunt generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size = 1 ) const override;
    void enableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    void disableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    Cunt getAttribLocation( Cunt programId, const String& attribName ) const override;
    void unbindBuffer( const BufferTypes bufferType ) const override;
    void bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const;
    Cunt generateBuffer( const BufferTypes type, const int size = 0 ) const override;

    void drawElements( const PrimitiveType type, const std::vector<unsigned int>& data ) const override;
    void drawElements( const PrimitiveType type, const std::vector<float>& data ) const override;
    void drawElementsFromLastBuffer( const PrimitiveType primitiveType, const DataType dataType, Cunt count ) const override;

    std::vector<std::string> listExtensions() override;

    void resetMatrixToIdentity( const MatrixTypes matrix ) const override;
    ~UtilConcrete();

protected:
private:
    UtilConcrete( const UtilConcrete& arg ) = delete;
    UtilConcrete( UtilConcrete&& arg ) = delete;
    UtilConcrete& operator=( const UtilConcrete& rhv ) = delete;
    UtilConcrete& operator=( UtilConcrete&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )