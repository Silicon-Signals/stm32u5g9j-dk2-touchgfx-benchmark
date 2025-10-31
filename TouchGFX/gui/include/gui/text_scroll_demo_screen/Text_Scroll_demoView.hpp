#ifndef TEXT_SCROLL_DEMOVIEW_HPP
#define TEXT_SCROLL_DEMOVIEW_HPP

#include <gui_generated/text_scroll_demo_screen/Text_Scroll_demoViewBase.hpp>
#include <gui/text_scroll_demo_screen/Text_Scroll_demoPresenter.hpp>

class Text_Scroll_demoView : public Text_Scroll_demoViewBase
{
public:
    Text_Scroll_demoView();
    virtual ~Text_Scroll_demoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);
    
protected:
};

#endif // TEXT_SCROLL_DEMOVIEW_HPP
