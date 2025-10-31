#ifndef IMAGE_DEMOPRESENTER_HPP
#define IMAGE_DEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class image_demoView;

class image_demoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    image_demoPresenter(image_demoView& v);

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

    virtual ~image_demoPresenter() {}

private:
    image_demoPresenter();

    image_demoView& view;
};

#endif // IMAGE_DEMOPRESENTER_HPP
