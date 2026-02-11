#ifndef RESULT_SCREENVIEW_HPP
#define RESULT_SCREENVIEW_HPP

#include <gui_generated/result_screen_screen/Result_screenViewBase.hpp>
#include <gui/result_screen_screen/Result_screenPresenter.hpp>

class Result_screenView : public Result_screenViewBase
{
public:
    Result_screenView();
    virtual ~Result_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateTextAreas(int ram_usage, int internal_flash, int external_flash, int renderTime, int cpu);
    void updateruntime_metrics(int fps, int cpu, int renderTime);

protected:
};

#endif // RESULT_SCREENVIEW_HPP
