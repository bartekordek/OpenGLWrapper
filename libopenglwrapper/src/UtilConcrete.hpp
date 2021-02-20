#pragma once

#include "libopenglwrapper/IUtility.hpp"

#include "CUL/Log/ILogger.hpp"

NAMESPACE_BEGIN( LOGLW )

class UtilConcrete final:
    public IUtility
{
public:
    UtilConcrete( CUL::CULInterface* culInterface );


    ~UtilConcrete();

protected:
private:
    void setProjection( const ProjectionData& rect ) const override;
    void setViewport( const Viewport& viewport ) const override;
    void setPerspective( const Angle& angle, CDouble widthToHeightRatio, CDouble m_zNear, CDouble m_zFar ) const override;
    void setOrthogonalPerspective( const ProjectionData& vp ) const override;
    void setPerspectiveProjection( const ProjectionData& vp ) const override;
    void lookAt( const ProjectionData& vp ) const override;
    void lookAt( const std::array< Pos3Dd, 3>& lookAtVec ) const override;
    void lookAt( const Pos3Dd& eye, const Pos3Dd& center, const Pos3Dd& up ) const override;

    ShaderTypes getShaderType( const CUL::String& fileExtension ) const override;

    unsigned int createProgram()  override;
    void removeProgram( Cunt programId ) const override;
    void useProgram( Cunt programId ) const override;
    void linkProgram( Cunt programId ) const override;
    void validateProgram( Cunt programId ) const override;

    unsigned int createShader( const IFile& shaderCode ) override;
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

// VBO, VAO
    void setVertexArrayClientState( const bool enable ) const override;
    unsigned int generateVertexArray( const int size = 1 ) const override;

    void bufferData( const CUL::MATH::Primitives::Quad& data, const BufferTypes type ) const override;
    void bufferData( const std::vector<unsigned int>& data, const BufferTypes type ) const override;
    void bufferData( const std::vector<float>& data, const BufferTypes type ) const override;
    void bufferData( const float vertices[] ) const override;

    unsigned int generateAndBindBuffer( const BufferTypes bufferType, const int size = 1 ) const override;
    unsigned int generateElementArrayBuffer( const std::vector<unsigned int>& data, const int size = 1 ) const override;
    void enableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    void disableVertexAttribiute( Cunt programId, const String& attribName ) const override;
    unsigned int getAttribLocation( Cunt programId, const String& attribName ) const override;
    void unbindBuffer( const BufferTypes bufferType ) const override;
    void bindBuffer( IVAO* vao ) const override;
    void bindBuffer( const BufferTypes bufferType, Cunt bufferId ) const override;
    unsigned int generateBuffer( const BufferTypes type, const int size = 1 ) const override;

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
    void setVertexPointer( int coordinatesPerVertex, DataType dataType, int stride, const void* data ) const override;

    std::vector<std::string> listExtensions() override;

    void resetMatrixToIdentity( const MatrixTypes matrix ) const override;

    void translate( const Point& point ) override;
    void translate( const float x, const float y, const float z ) override;
    void rotate(
        const float angle,
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f ) override;

    virtual void scale( const CUL::MATH::Vector3Df& scale ) const override;
    virtual void scale( const float scale ) const override;





    void draw( const Triangle& triangle, const ColorS& color ) override;
    void draw( const Triangle& quad, const std::array<ColorS, 4>& color ) override;
    void draw( const TriangleData& values, const std::array<ColorS, 3>& color ) override;

    void draw( const Quad& quad, const Quad& texQuad ) override;

    void draw( const Quad& quad, const ColorS& color ) override;
    void draw( const Quad& quad, const std::array<ColorS, 4>& color ) override;

    void draw( const QuadData& quad, const ColorS& color ) override;
    void draw( const QuadData& quad, const std::array<ColorS, 4>& color ) override;

    void assertOnProgramError( Cunt programId, Cunt val ) const;
    void log( const String& text,
            const CUL::LOG::Severity severity = CUL::LOG::Severity::INFO ) const;
    void assert( const bool value, const CUL::String& message ) const;
    void setDepthTest( const bool enabled ) const override;
    void setBackfaceCUll( const bool enabled ) const override;

// Texturing
    void setTexuring( const bool enabled ) const override;
    unsigned generateTexture() const override;
    void bindTexture( const unsigned int textureId ) const override;
    void setTextureParameter(
        const TextureParameters type,
        const TextureFilterType val ) const override;
    void setTextureData( const TextureInfo& ti ) const override;
    void freeTexture( unsigned int& textureId ) const override;

    void matrixStackPush() override;
    void matrixStackPop() override;

    CUL::CULInterface* m_culInterface = nullptr;
    CUL::LOG::ILogger* m_logger = nullptr;
    unsigned int m_currentMatrix = 0;

    UtilConcrete( const UtilConcrete& arg ) = delete;
    UtilConcrete( UtilConcrete&& arg ) = delete;
    UtilConcrete& operator=( const UtilConcrete& rhv ) = delete;
    UtilConcrete& operator=( UtilConcrete&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )