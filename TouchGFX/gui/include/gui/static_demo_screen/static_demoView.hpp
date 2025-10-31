#ifndef STATIC_DEMOVIEW_HPP
#define STATIC_DEMOVIEW_HPP

#include <gui_generated/static_demo_screen/static_demoViewBase.hpp>
#include <gui/static_demo_screen/static_demoPresenter.hpp>
#include <gui/common/RoundedRect.hpp>

class static_demoView : public static_demoViewBase
{
public:
    static_demoView();
    virtual ~static_demoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);
    
protected:
    RoundedRect rectangle;
    int tickCounter;  // Track frames
    static const int TOTAL_DURATION_TICKS = 600;  // 10 seconds at 60 FPS
    static const int TICKS_PER_COLOR_CHANGE = 12;
};

#endif // STATIC_DEMOVIEW_HPP
