#ifndef STATIC_DEMOPRESENTER_HPP
#define STATIC_DEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class static_demoView;

class static_demoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    static_demoPresenter(static_demoView& v);

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

    virtual ~static_demoPresenter() {}

private:
    static_demoPresenter();

    static_demoView& view;
};

#endif // STATIC_DEMOPRESENTER_HPP
