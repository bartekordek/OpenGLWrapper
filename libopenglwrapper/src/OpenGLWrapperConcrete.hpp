#pragma once

#include "libopenglwrapper/IOpenGLWrapper.hpp"
#include "libopenglwrapper/IDebugOverlay.hpp"
#include "OpenGLShaderFactory.hpp"

#include "SDL2Wrapper/IMPORT_SDL_video.hpp"
#include "SDL2Wrapper/ISDLEventObserver.hpp"

#include "CUL/GenericUtils/LckPrim.hpp"
#include "CUL/ThreadUtils.hpp"
#include "CUL/GenericUtils/DumbPtr.hpp"
#include "CUL/GenericUtils/ITask.hpp"
#include "CUL/JSON/IJSONFile.hpp"
#include "CUL/ITimer.hpp"

#include "CUL/STL_IMPORTS/STD_queue.hpp"
#include "CUL/STL_IMPORTS/STD_set.hpp"
#include "CUL/STL_IMPORTS/STD_array.hpp"
#include "CUL/STL_IMPORTS/STD_thread.hpp"
#include "CUL/STL_IMPORTS/STD_variant.hpp"

union SDL_Event;

NAMESPACE_BEGIN( LOGLW )

using ITask = CUL::GUTILS::ITask;
using SafeBool = CUL::GUTILS::LckPrim<bool>;
template <typename Type> using DumbPtr = CUL::GUTILS::DumbPtr<Type>;
template <typename Type> using Safe = CUL::GUTILS::LckPrim<Type>;

enum class DebugType
{
    TEXT = 0,
    SLIDER
};

enum class DataTypes
{
    NONE = -1,
    INT,
    UNSIGNED_INT,
    STRING,
    FLOAT,
    DOUBLE
};

struct DebugValueRow
{
    unsigned id = 0;
    String text;
    std::variant<String*, float*, int*> value;
    DebugType type = DebugType::TEXT;
    float min = 0.0f;
    float max = 0.0f;
    std::function<void( void )> onChange;
};

class OpenGLWrapperConcrete final:
    public IOpenGLWrapper,
    private IObjectFactory,
    private IDebugOverlay,
    private SDL2W::ISDLEventObserver
{
public:
    OpenGLWrapperConcrete( SDL2W::ISDL2Wrapper* sdl2w );
    ~OpenGLWrapperConcrete();

protected:
private:
    void initialize() override;
    void showExtensions();
    void setupProjectionData( const SDL2W::WindowSize& winSize );
    CUL::CULInterface* getCul() override;

    void refreshBuffers();
    void setProjection( const ProjectionData& rect ) override;
    void setViewport( const Viewport& viewport, const bool instant = false ) override;

    IRect* createRect() override;
    Triangle* createTriangle() override;
    IObject* createFromFile( const String& path ) override;
    IObject* createFromFile( CUL::JSON::IJSONFile* file );
    IObject* createFromFile( IFile* file ) override;
    IObject* createTriangle( CUL::JSON::INode* jNode );

    void setBackgroundColor( const ColorS& color ) override;
    void startRenderingLoop() override;
    void stopRenderingLoop() override;
    void onInitialize( const EmptyFunctionCallback& callback ) override;
    void beforeFrame( const EmptyFunctionCallback& callback ) override;

    void addObjectToRender( IRenderable* renderable ) override;

    IShaderFactory* getShaderFactory() override;
    IObjectFactory* getObjectFactory() override;
    IProgramFactory* getProgramFactory() override;
    IImageLoader* getImageLoader() override;
    CUL::LOG::ILogger* getLoger() override;
    IUtility* getUtility() override;
    const Viewport& getViewport() const override;
    const ProjectionData* const getProjectionData() const override;

    const ContextInfo& getContext() const override;

    void mainThread();
    void renderLoop();
    void calculateNextFrameLengths();
    void initDebugInfo();
    void renderFrame() override;
    void renderInfo();
    void setProjectionType( const ProjectionType type ) override;
    void changeProjectionType();
    void prepareProjection();
    void setEyePos( const Pos3Df& pos ) override;
    void renderObjects();

    void executeTasks();

    void release();

    void drawQuad( const bool draw = true ) override;

    void clearModelViewEveryFrame( const bool enable ) override;
    void calculateFrameWait();

    void drawDebugInfo( const bool enable ) override;

    IDebugOverlay* getDebugOverlay() override;
    void handleEvent( const SDL_Event& event ) override;

    unsigned addSliderValue( const CUL::String& valName, float* value, float min, float max, const std::function<void( void )> onUpdate = nullptr ) override;
    unsigned addText( const CUL::String& text, float* value ) override;

    std::map<unsigned, DebugValueRow> m_debugValues;

    std::atomic<bool> m_enableDebugDraw = false;
    std::atomic<bool> m_debugDrawInitialized = false;

    DumbPtr<OpenGLShaderFactory> m_shaderFactory;
    ContextInfo m_glContext;

    SDL2W::ISDL2Wrapper* m_sdlW = nullptr;
    SDL2W::IWindow* m_activeWindow = nullptr;
    CUL::CULInterface* m_cul = nullptr;
    CUL::LOG::ILogger* m_logger = nullptr;
    IUtility* m_oglUtility = nullptr;

    DumbPtr<IImageLoader> m_imageLoader;

    std::thread m_renderingLoopThread;

    CUL::GUTILS::ValueChangeHook<bool> m_isPerspective = true;
    SafeBool m_runRenderLoop = true;
    SafeBool m_clearEveryFrame = true;
    SafeBool m_clearModelView = true;
    SafeBool m_updateBuffers = true;

    Viewport m_viewport;
    SafeBool m_viewportChanged = false;

    ProjectionData m_projectionData;
    SafeBool m_projectionChanged = false;
    Safe<ProjectionType> m_currentProjection = ProjectionType::ORTO;

    ColorS m_backgroundColor = ColorS( ColorE::BLACK );

    std::queue<ITask*> m_preRenderTasks;

    std::mutex m_objectsToRenderMtx;
    std::set<IRenderable*> m_objectsToRender;

    EmptyFunctionCallback m_onInitializeCallback;
    EmptyFunctionCallback m_onBeforeFrame;

    DumbPtr<CUL::ITimer> m_frameTimer;
    Safe<float> m_fpsLimit = 60.f;
    Safe<int> m_targetFrameLengthUs = 0u;
    Safe<int> m_currentFrameLengthUs = 0u;
    Safe<int> m_frameSleepUs = 64;
    int m_usRes = 0;
    int m_usDelta = 8;

    Safe<int> m_waitTimeUs = 0.0f;

    bool m_hasBeenInitialized = false;
    SafeBool m_drawQuad = false;

private: // Deleted
    OpenGLWrapperConcrete() = delete;
    OpenGLWrapperConcrete( const OpenGLWrapperConcrete& val ) = delete;
    OpenGLWrapperConcrete( OpenGLWrapperConcrete&& val ) = delete;
    OpenGLWrapperConcrete& operator=( const OpenGLWrapperConcrete& rhv ) = delete;
    OpenGLWrapperConcrete& operator=( OpenGLWrapperConcrete&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )