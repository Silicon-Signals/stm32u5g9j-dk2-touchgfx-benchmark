#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }
/*
    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }
*/
    virtual void handleTickEvent();

    void startAutoDemo();

    bool StartButton = false;
    bool isAutoDemoRunning = false;
    
private:
    enum DemoStep : uint8_t {
        DEMO_IDLE = 0,
        DEMO_VIDEO,
        DEMO_IMAGE,
        DEMO_STATIC,
        DEMO_SVG,
        DEMO_TEXT_SCROLL,
        DEMO_CLUSTER,
    };

    DemoStep currentDemo = DEMO_IDLE;
    uint32_t demoTickCounter = 0;
    static constexpr uint32_t TICKS_13_SEC = 13 * 60;   // assuming 60 fps
};

#endif // FRONTENDAPPLICATION_HPP
