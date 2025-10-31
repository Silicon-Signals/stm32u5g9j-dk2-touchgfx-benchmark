#ifndef IMAGE_DEMO_OLDPRESENTER_HPP
#define IMAGE_DEMO_OLDPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class image_demo_oldView;

class image_demo_oldPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    image_demo_oldPresenter(image_demo_oldView& v);

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

    virtual ~image_demo_oldPresenter() {}

private:
    image_demo_oldPresenter();

    image_demo_oldView& view;
};

#endif // IMAGE_DEMO_OLDPRESENTER_HPP
