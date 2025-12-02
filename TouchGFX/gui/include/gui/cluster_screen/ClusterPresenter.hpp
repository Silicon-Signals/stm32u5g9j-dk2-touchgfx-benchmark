#ifndef CLUSTERPRESENTER_HPP
#define CLUSTERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ClusterView;

class ClusterPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ClusterPresenter(ClusterView& v);

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

    virtual ~ClusterPresenter() {}

private:
    ClusterPresenter();

    ClusterView& view;
};

#endif // CLUSTERPRESENTER_HPP
