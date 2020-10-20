#include "ProgramConcrete.hpp"
#include "VAOOpengl.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "CUL/Filesystem/FileFactory.hpp"

using namespace LOGLW;

ProgramConcrete::ProgramConcrete( IUtility* utility, IShaderFactory& sf ):
    m_utility( utility ),
    m_id( utility->createProgram() ),
    m_sf( sf )
{
    CUL::Assert::simple( m_id != 0, "Cannot create program." );
}


IVAO* ProgramConcrete::createVao()
{
    auto vao = IVAO::createVAO();
    m_vaoList.push_back( vao );
    return vao;
}


void ProgramConcrete::setAttrib( const String&, const char* )
{
    //TODO
}

void ProgramConcrete::setAttrib( const String& , Cfloat  )
{
}

void ProgramConcrete::setAttrib( const String& , Cunt  )
{
}

void ProgramConcrete::setAttrib( const String& , Cint  )
{
}

const String ProgramConcrete::getAttributeStr( const String&  )
{
    return String();
}

Cfloat ProgramConcrete::getAttributeF( const String&  )
{
    return Cfloat();
}

Cunt ProgramConcrete::getAttributeUi( const String&  )
{
    return Cunt();
}

Cint ProgramConcrete::getAttributeI( const String&  )
{
    return Cint();
}

void ProgramConcrete::attachShader( IShader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    m_utility->attachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = shader;
}

void ProgramConcrete::dettachShader( IShader* shader )
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    auto shaderId = shader->getId();
    m_utility->dettachShader( m_id, shaderId );
    m_attachedShaders[shader->getPath()] = nullptr;
}

void ProgramConcrete::link()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->linkProgram( m_id );
}

void ProgramConcrete::enable()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->useProgram( m_id );
}

void ProgramConcrete::disable()
{
    m_utility->useProgram( 0 );
}

void ProgramConcrete::validate()
{
    std::lock_guard<std::mutex> lock( m_operationMutex );
    m_utility->validateProgram( m_id );
}

IShader* ProgramConcrete::createShader( const Path& path )
{
    std::unique_ptr<IFile> filePtr( CUL::FS::FileFactory::createFileFromPath( path ) );
    filePtr->load( true );
    return createShader( filePtr.get() );
}

IShader* ProgramConcrete::createShader( IFile* file )
{
    auto result = m_sf.createShader( file );
    attachShader( result );
    link();
    return result;
}

void ProgramConcrete::render()
{
}

const ProgramConcrete::AttribKey ProgramConcrete::getAttribLocation( const String& name ) const
{
    ProgramConcrete::AttribKey result;

    auto it = m_attribMap.find( name );
    if( it == m_attribMap.end() )
    {
        result = m_utility->getAttribLocation( m_id, name );
    }
    else
    {
        result = it->second;
    }

    return result;
}

const ShaderList& ProgramConcrete::getShaderList() const
{
    return m_attachedShaders;
}

// TODO
void ProgramConcrete::bufferData( const std::vector<float>&, const BufferTypes )
{
}

Cunt ProgramConcrete::getProgramId() const
{
    return m_id;
}

ProgramConcrete::~ProgramConcrete()
{
    m_utility->removeProgram( m_id );
    m_id = 0;
}