#include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::handleTickEvent()
{
    FrontendApplicationBase::handleTickEvent();

    if (currentDemo == DEMO_IDLE) {
        return;
    }

    demoTickCounter++;

    // Different timeout depending on current demo
    uint32_t requiredTicks = TICKS_13_SEC;

    if (currentDemo == DEMO_CLUSTER) {
        requiredTicks = 23UL * 60;
    }

    if (currentDemo == DEMO_TEXT_SCROLL) {
        requiredTicks = 16UL * 30;
    }

    if (demoTickCounter >= requiredTicks)
    {
        demoTickCounter = 0;

        switch (currentDemo)
        {
            case DEMO_VIDEO:
                currentDemo = DEMO_IMAGE;
                gotoimage_demoScreenNoTransition();
                break;

            case DEMO_IMAGE:
                currentDemo = DEMO_STATIC;
                gotostatic_demoScreenNoTransition();
                break;

            case DEMO_STATIC:
                currentDemo = DEMO_SVG;
                gotoSVG_demoScreenNoTransition();
                break;

            case DEMO_SVG:
                currentDemo = DEMO_TEXT_SCROLL;
                gotoText_Scroll_demoScreenNoTransition();
                break;

            case DEMO_TEXT_SCROLL:
                currentDemo = DEMO_CLUSTER;
                gotoClusterScreenNoTransition();
                break;

            case DEMO_CLUSTER:                
                currentDemo = DEMO_VIDEO;
                gotovideo_demoScreenNoTransition();
                break;

            default:
                break;
        }
    }
}

void FrontendApplication::startAutoDemo()
{
    isAutoDemoRunning = true;
    currentDemo = DEMO_VIDEO;
    demoTickCounter = 0;
    gotovideo_demoScreenNoTransition();
}
