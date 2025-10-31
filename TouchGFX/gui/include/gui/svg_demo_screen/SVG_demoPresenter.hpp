#ifndef SVG_DEMOPRESENTER_HPP
#define SVG_DEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SVG_demoView;

class SVG_demoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SVG_demoPresenter(SVG_demoView& v);

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

    virtual ~SVG_demoPresenter() {}

private:
    SVG_demoPresenter();

    SVG_demoView& view;
};

#endif // SVG_DEMOPRESENTER_HPP
