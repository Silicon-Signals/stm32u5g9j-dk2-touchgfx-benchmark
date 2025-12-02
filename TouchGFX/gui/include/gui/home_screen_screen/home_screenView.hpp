#ifndef HOME_SCREENVIEW_HPP
#define HOME_SCREENVIEW_HPP

#include <gui_generated/home_screen_screen/home_screenViewBase.hpp>
#include <gui/home_screen_screen/home_screenPresenter.hpp>

class home_screenView : public home_screenViewBase
{
public:
    home_screenView();
    virtual ~home_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);
    virtual void runStartButtonAnimation();

protected:
};

#endif // HOME_SCREENVIEW_HPP
