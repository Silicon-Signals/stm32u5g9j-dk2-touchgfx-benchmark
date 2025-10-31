#ifndef IMAGE_DEMOVIEW_HPP
#define IMAGE_DEMOVIEW_HPP

#include <gui_generated/image_demo_screen/image_demoViewBase.hpp>
#include <gui/image_demo_screen/image_demoPresenter.hpp>

class image_demoView : public image_demoViewBase
{
public:
    image_demoView();
    virtual ~image_demoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);

protected:
    float zAngle2D;
    float deltaZangle2D;
};

#endif // IMAGE_DEMOVIEW_HPP
