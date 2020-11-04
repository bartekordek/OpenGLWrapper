#pragma once

#include "libopenglwrapper/IUtility.hpp"

#include "CUL/Log/ILogger.hpp"

NAMESPACE_BEGIN( LOGLW )

class UtilConcrete final:
    public IUtility
{
public:
    UtilConcrete( CUL::CULInterface* culInterface );

    void setProjection( const ProjectionData& rect ) const override;
    void setViewport( const Viewport& viewport ) const override;
    void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar ) const override;
    void setOrthogonalPerspective( const ProjectionData& vp ) const override;
    void setPerspectiveProjection( const ProjectionData& vp ) const override;
    void lookAt( const ProjectionData& vp ) const override;
    void lookAt( const std::array< Pos3Dd, 3>& lookAtVec ) const override;
    void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const override;

    const ShaderTypes getShaderType( const CUL::String& fileExtension ) const override;

    Cunt createProgram() const override;
    void removeProgram( Cunt programId ) const override;
    void useProgram( Cunt programId ) const override;
    void linkProgram( Cunt programId ) const override;
    void validateProgram( Cunt programId ) const override;

    Cunt createShader( const IFile& shaderCode ) const override;
    void attachShader( Cunt programId, Cunt shaderId ) const override;
    void dettachShader( Cunt programId, Cunt shaderId ) const override;
    void removeShader( Cunt shaderId ) const override;

    ContextInfo initContextVersion( SDL2W::IWindow* window, Cunt major, Cunt minor ) const override;
    void destroyContext( ContextInfo& context ) override;
    void setAttribValue( Cint attributeLocation, Cfloat value ) const override;
    void setAttribValue( Cint attributeLocation, Cint value ) const override;
    void setAttribValue( Cint attributeLocation, Cunt value ) const override;

    void setProjectionAndModelToIdentity() const override;
    void clearColorAndDepthBuffer() const override; 
    void createQuad( Cfloat scale = 1.0f ) const override;
    void clearColorTo( const ColorS color ) const override;
    void clearBuffer( const ClearMasks mask ) const override;

    Cunt generateVertexArray (const int size = 1 ) const override;

    void bufferData( const std::vector<unsigned int>& data, const BufferTypes type ) const override;
    void bufferData( const std::vector<float>& data, const BufferTypes type ) const override;
    void bufferData( const float vertices[] ) const override;

    Cunt generateAndBindBuffer( const BufferTypes bufferType, const int size = 1 ) const override;
    Cunt generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size = 1 ) const override;
    void enableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    void disableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    Cunt getAttribLocation( Cunt programId, const String& attribName ) const override;
    void unbindBuffer( const BufferTypes bufferType ) const override;
    void bindBuffer(IVAO* vao) const override;
    void bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const override;
    Cunt generateBuffer( const BufferTypes type, const int size = 1 ) const override;

    void drawElements( const PrimitiveType type, const std::vector<unsigned int>& data ) const override;
    void drawElements( const PrimitiveType type, const std::vector<float>& data ) const override;
    void drawElementsFromLastBuffer( const PrimitiveType primitiveType, const DataType dataType, Cunt count ) const override;
    void drawArrays( const PrimitiveType primitiveType, Cunt first, Cunt count ) const override;
    void vertexAttribPointer(
        Cunt vertexAttributeId,
        Cint componentsPerVertexAttribute,
        const DataType dataType,
        Cbool normalized,
        Cint stride,
        const void* offset = nullptr ) const override;
    void enableVertexAttribArray( Cunt attributeId ) const override;

    std::vector<std::string> listExtensions() override;

    void resetMatrixToIdentity( const MatrixTypes matrix ) const override;

    void translate( const float x, const float y, const float z ) override;
    void rotate(
        const float angle,
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f ) override;

    void draw( const QuadF& quad, const ColorS& color ) override;
    void draw( const QuadF& quad, const std::array<ColorS, 4>& color ) override;

    void draw( const TriangleF& triangle, const ColorS& color ) override;
    void draw( const TriangleF& quad, const std::array<ColorS, 4>& color ) override;

    ~UtilConcrete();

protected:
private:
    void assertOnProgramError( Cunt programId, Cunt val ) const;
    void log( const String& text,
              const CUL::LOG::Severity severity = CUL::LOG::Severity::INFO ) const;
    void assert( const bool value, const CUL::String& message ) const;
    void setDepthTest( const bool enabled ) const override;

    CUL::CULInterface* m_culInterface = nullptr;
    CUL::LOG::ILogger* m_logger = nullptr;

    UtilConcrete( const UtilConcrete& arg ) = delete;
    UtilConcrete( UtilConcrete&& arg ) = delete;
    UtilConcrete& operator=( const UtilConcrete& rhv ) = delete;
    UtilConcrete& operator=( UtilConcrete&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )