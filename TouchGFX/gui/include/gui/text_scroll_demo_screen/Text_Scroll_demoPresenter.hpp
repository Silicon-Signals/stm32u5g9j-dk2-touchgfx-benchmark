#ifndef TEXT_SCROLL_DEMOPRESENTER_HPP
#define TEXT_SCROLL_DEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Text_Scroll_demoView;

class Text_Scroll_demoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Text_Scroll_demoPresenter(Text_Scroll_demoView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Text_Scroll_demoPresenter() {}

private:
    Text_Scroll_demoPresenter();

    Text_Scroll_demoView& view;
};

#endif // TEXT_SCROLL_DEMOPRESENTER_HPP
