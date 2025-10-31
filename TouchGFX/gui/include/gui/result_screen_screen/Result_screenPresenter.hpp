#ifndef RESULT_SCREENPRESENTER_HPP
#define RESULT_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Result_screenView;

class Result_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Result_screenPresenter(Result_screenView& v);

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

    virtual ~Result_screenPresenter() {}

private:
    Result_screenPresenter();

    Result_screenView& view;
};

#endif // RESULT_SCREENPRESENTER_HPP
