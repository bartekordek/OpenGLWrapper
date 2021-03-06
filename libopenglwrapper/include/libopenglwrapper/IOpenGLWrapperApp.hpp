#pragma once

#include "libopenglwrapper/IOpenGLWrapper.hpp"

#include "SDL2Wrapper/ISDL2Wrapper.hpp"
#include "SDL2Wrapper/Input/IKeyboardObserver.hpp"
#include "SDL2Wrapper/Input/IMouseObserver.hpp"

template <typename TYPE>
using Ptr = CUL::GUTILS::DumbPtr<TYPE>;

NAMESPACE_BEGIN(LOGLW)

class IOpenGLWrapper;

class LIBOPENGLWRAPPER_API IOpenGLWrapperApp:
    private SDL2W::IWindowEventObserver,
    private SDL2W::IKeyboardObserver,
    private SDL2W::IMouseObserver
{
public:
    IOpenGLWrapperApp( bool fullscreen, unsigned width, unsigned height, int x, int y, const char* winName, const char* configPath );

    void run();
    void close();

    virtual ~IOpenGLWrapperApp();
protected:
    LOGLW::IObjectFactory* m_objFactory = nullptr;
    CUL::LOG::ILogger* m_logger = nullptr;
    LOGLW::IUtility* m_gutil = nullptr;
    LOGLW::ProjectionData m_projectionData;


    Ptr<SDL2W::ISDL2Wrapper> m_sdlw;
    Ptr<LOGLW::IOpenGLWrapper> m_oglw;
private:
    // Override these to add own events handling.
    virtual void onWindowEvent(const SDL2W::WindowEvent::Type e) override;
    virtual void onKeyBoardEvent(const SDL2W::IKey& key) override;
    virtual void onMouseEvent(const SDL2W::MouseData& md) override;
    virtual void onInit() {};
    virtual void customFrame() {};


};
NAMESPACE_END( LOGLW )