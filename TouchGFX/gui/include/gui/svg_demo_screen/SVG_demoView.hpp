#ifndef SVG_DEMOVIEW_HPP
#define SVG_DEMOVIEW_HPP

#include <gui_generated/svg_demo_screen/SVG_demoViewBase.hpp>
#include <gui/svg_demo_screen/SVG_demoPresenter.hpp>

class SVG_demoView : public SVG_demoViewBase
{
public:
    SVG_demoView();
    virtual ~SVG_demoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    
    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);
    
protected:
   
};

#endif // SVG_DEMOVIEW_HPP
