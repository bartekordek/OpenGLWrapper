#pragma once

#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "OpenGLShaderFactory.hpp"

#include "SDL2Wrapper/IMPORT_SDL_video.hpp"

#include "CUL/STL_IMPORTS/STD_thread.hpp"
#include "CUL/GenericUtils/LckPrim.hpp"
#include "CUL/ThreadUtils.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/STL_IMPORTS/STD_queue.hpp"
#include "CUL/STL_IMPORTS/STD_set.hpp"
#include "CUL/GenericUtils/ITask.hpp"

NAMESPACE_BEGIN( LOGLW )

using ITask = CUL::GUTILS::ITask;
using SafeBool = CUL::GUTILS::LckPrim<bool>;

class OpenGLWrapperConcrete final:
    public IOpenGLWrapper,
    private IObjectFactory
{
public:
    OpenGLWrapperConcrete(
        SDL2W::IWindow* window,
        SDL2W::ISDL2Wrapper* sdl2w );
    ~OpenGLWrapperConcrete();
    
    void renderFrame() override;
    void renderObjects();
    void refreshBuffers();
    void setBackgroundColor( const ColorS& color ) override;
    void startRenderingLoop() override;
    void stopRenderingLoop() override;
    void onInitialize( const std::function<void()>& callback ) override;
    void beforeFrame( const std::function<void()>& callback ) override;

    IShaderFactory* getShaderFactory() override;
    IObjectFactory* getObjectFactory() override;
    IProgramFactory* getProgramFactory() override;

    CUL::LOG::ILogger* getLoger() override;

protected:
private:
    IRect* createRect() override;
    ITriangle* createTriangle() override;

    OpenGLWrapperConcrete() = delete;
    OpenGLWrapperConcrete(
        const OpenGLWrapperConcrete& val) = delete;
    OpenGLWrapperConcrete& operator=(
        const OpenGLWrapperConcrete& rhv) = delete;

    void renderLoop();
    void initialize();
    void executeTasks();

    CUL::GUTILS::DumbPtr<OpenGLShaderFactory> m_shaderFactory;

    SDL2W::ISDL2Wrapper* m_sdlW = nullptr;
    SDL2W::IWindow* m_activeWindow = nullptr;
    SDL_GLContext m_oglContext = nullptr;

    std::thread m_renderingLoopThread;

    SafeBool m_runRenderLoop = true;
    SafeBool m_clearEveryFrame = true;
    SafeBool m_clearModelView = true;
    SafeBool m_updateBuffers = true;

    ColorS m_backgroundColor;

    std::queue<ITask*> m_preRenderTasks;
    std::set<IRenderable*> m_objectsToRender;

    std::function<void()> m_onInitializeCallback;
    std::function<void()> m_onBeforeFrame;

    bool m_hasBeenInitialized = false;

    CUL::LOG::ILogger* m_logger = nullptr;

};

NAMESPACE_END( LOGLW )