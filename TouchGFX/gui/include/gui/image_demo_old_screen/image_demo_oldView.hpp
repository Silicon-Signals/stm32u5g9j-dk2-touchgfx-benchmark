#ifndef IMAGE_DEMO_OLDVIEW_HPP
#define IMAGE_DEMO_OLDVIEW_HPP

#include <gui_generated/image_demo_old_screen/image_demo_oldViewBase.hpp>
#include <gui/image_demo_old_screen/image_demo_oldPresenter.hpp>

class image_demo_oldView : public image_demo_oldViewBase
{
public:
    image_demo_oldView();
    virtual ~image_demo_oldView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    int tickCounter;  
    int currentPositionIndex;  
    static const int TOTAL_DURATION_TICKS = 600;  
    static const int POSITIONS_COUNT = 16;
    static const int TICKS_PER_MOVE = 30;
    struct Position { int x; int y; };
    Position positions[POSITIONS_COUNT];
};

#endif // IMAGE_DEMO_OLDVIEW_HPP
