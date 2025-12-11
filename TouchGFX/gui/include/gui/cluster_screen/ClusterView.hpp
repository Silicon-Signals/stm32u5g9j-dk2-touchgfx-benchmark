#ifndef CLUSTERVIEW_HPP
#define CLUSTERVIEW_HPP

#include <gui_generated/cluster_screen/ClusterViewBase.hpp>
#include <gui/cluster_screen/ClusterPresenter.hpp>

class ClusterView : public ClusterViewBase
{
public:
    ClusterView();
    virtual ~ClusterView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void startBothAnimations();
    virtual void handleTickEvent();
    void updateruntime_metrics(int fps, int cpu, int renderTime);
    
protected:

    // CLUSTER ANIMATION
    uint16_t animation_frame;
    uint16_t animation_tickcounter;
    bool left_animation_running;
    bool right_animation_running;
    static const uint16_t ANIM_TOTAL_FRAMES = 40; // 000 to 039
    static const uint8_t ANIM_SPEED = 6;

    // 20-SECOND TIMER
    uint16_t tickCounter = 0;                   // counts ticks (60 = 1 sec)
    const uint16_t TOTAL_TICKS = 20 * 60;       // 20 seconds = 1200 ticks @ 60 FPS
    const uint16_t TICKS_PER_PHASE = 2 * 60;

    // Indicator blink timer
    const uint16_t INDICATOR_CYCLE_TICKS = 6 * 60;  // 6 seconds per cycle (at 60 FPS)

    // DIPPER blink timer
    bool dipperOn = false;
    uint16_t dipper_flashtimer = 0;
    uint8_t  dipper_flashcount = 0;
    const uint16_t DIPPER_FLASH_DURATION = 30;

    enum class IndicatorState
    {
        LeftOnly,
        RightOnly,
        BothOn
    };
    IndicatorState indicatorState = IndicatorState::LeftOnly;

    // Cluster frame timer
    enum class Phase
    {
        Forward1,
        Reverse1,
        Forward2,
        Reverse2,
        Forward3
    };
    Phase phase = Phase::Forward1;

};

#endif // CLUSTERVIEW_HPP
