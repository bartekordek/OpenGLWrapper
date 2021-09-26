#pragma once

#include "libopenglwrapper/Program.hpp"
#include "libopenglwrapper/IRenderable.hpp"
#include "libopenglwrapper/VertexBuffer.hpp"
#include "libopenglwrapper/Shader.hpp"

#include "CUL/STL_IMPORTS/STD_cstdint.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_mutex.hpp"
#include "CUL/STL_IMPORTS/STD_queue.hpp"

/*
A Vertex Array Object (VAO) is an OpenGL Object
that stores all of the state needed to supply
vertex data (with one minor exception noted below).
It stores the format of the vertex data as well as
the Buffer Objects (see below) providing the vertex data arrays.
Note that VAOs do not copy,
freeze or store the contents of the referenced buffers - if you
change any of the data in the buffers referenced by an existing VAO,
those changes will be seen by users of the VAO.

A Vertex Array Object (VAO) is an object which contains
one or more Vertex Buffer Objects and is designed to
store the information for a complete rendered object.
In our example this is a diamond consisting of four
vertices as well as a color for each vertex.
*/

// Vertex Array Objects (VAOs) are conceptually nothing but thin state wrappers.
// VAO - attribs?

NAMESPACE_BEGIN( LOGLW )

using BuffIDType = uint8_t;

class LIBOPENGLWRAPPER_API VertexArray final:
    public IUtilityUser,
    public IRenderable
{
public:
    explicit VertexArray(bool createOnRender = true);

    BuffIDType getId() const;
    void addVBO( VertexBuffer* vbo );

    void addVertexBuffer( VertexBufferData& data, bool instant = false );

    void createShader( const CUL::FS::Path& path );

    Program* getProgram();

    void render() override;

    VertexBuffer* getVertexBuffer();

    ~VertexArray();
protected:
private:
    enum class TaskType : short
    {
        NONE = 0,
        CREATE_VAO,
        ADD_VBO,
        CREATE_PROGRAM,
        ADD_SHADER,
        RENDER
    };

    bool taskIsAlreadyPlaced( TaskType tt ) const;

    void runTasks();
    void registerTask( TaskType taskType );

    std::mutex m_vbosMtx;


    void createVAO();
    void createVBOs();

    void bind();
    void unbind();
    void release();

    unsigned m_bufferId = 0;

    std::mutex m_tasksMtx;
    std::deque<TaskType> m_tasks;

    Program m_shaderProgram;
    std::vector<Ptr<Shader>> m_shaders;
    std::mutex m_shadersMtx;
    std::queue<CUL::FS::Path> m_shadersPaths;

    std::vector<VertexBufferData> m_vboDataToPrepare;
    std::vector<Ptr<VertexBuffer>> m_vbos;
    size_t m_vbosCount = 0;

    std::vector<Ptr<IndexBuffer>> m_indexBuffers;

    std::vector < std::vector<unsigned>> m_indicesToPrepare;

    VertexArray( const VertexArray& value ) = delete;
    VertexArray( VertexArray&& value ) = delete;
    VertexArray& operator=( const VertexArray& value ) = delete;
    VertexArray& operator=( VertexArray&& value ) = delete;
};

NAMESPACE_END( LOGLW )