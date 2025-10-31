#ifndef VIDEO_DEMOPRESENTER_HPP
#define VIDEO_DEMOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class video_demoView;

class video_demoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    video_demoPresenter(video_demoView& v);

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

    virtual ~video_demoPresenter() {}

private:
    video_demoPresenter();

    video_demoView& view;
};

#endif // VIDEO_DEMOPRESENTER_HPP
