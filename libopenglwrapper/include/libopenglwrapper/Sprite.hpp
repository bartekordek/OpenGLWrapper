#pragma once

#include "libopenglwrapper/IObject.hpp"

#include "CUL/Graphics/IImage.hpp"

#include "CUL/STL_IMPORTS/STD_deque.hpp"

NAMESPACE_BEGIN( LOGLW )

class LIBOPENGLWRAPPER_API Sprite final:
    public IObject,
    public IUtilityUser
{
public:
    Sprite();

    CUL::Graphics::IImage* m_image = nullptr;
    unsigned m_textureId = 0;

    void render() override;

    ~Sprite();
protected:
private:
    enum class TaskType : short
    {
        CREATE_VAO = 1
    };

    void renderModern();
    void renderLegacy();
    void runTasks();
    void registerTask( TaskType taskType );
    bool taskIsAlreadyPlaced( TaskType tt ) const;

    CUL::GUTILS::DumbPtr<class VertexArray> m_vao;

    std::mutex m_tasksMtx;
    std::deque<TaskType> m_tasks;

// Deleted:
    Sprite( const Sprite& arg ) = delete;
    Sprite( Sprite&& arg ) = delete;
    Sprite& operator=( const Sprite& rhv ) = delete;
    Sprite& operator=( Sprite&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )