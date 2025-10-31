#ifndef VIDEO_DEMOVIEW_HPP
#define VIDEO_DEMOVIEW_HPP

#include <gui_generated/video_demo_screen/video_demoViewBase.hpp>
#include <gui/video_demo_screen/video_demoPresenter.hpp>

class video_demoView : public video_demoViewBase
{
public:
    video_demoView();
    virtual ~video_demoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);

protected:
    int tickCounter;
    static const int TOTAL_DURATION_TICKS = 600;
};

#endif // VIDEO_DEMOVIEW_HPP
