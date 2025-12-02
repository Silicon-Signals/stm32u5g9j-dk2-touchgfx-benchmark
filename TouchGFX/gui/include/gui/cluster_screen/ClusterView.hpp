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

    // === CLUSTER ANIMATION ===
    bool isForward;

    uint16_t leftAnimFrame;
    uint16_t leftAnimTickCounter;
    bool leftAnimationRunning;

    uint16_t rightAnimFrame;
    uint16_t rightAnimTickCounter;
    bool rightAnimationRunning;

    // === 20-SECOND TIMER ===
    uint16_t totalTimer = 0;                     // counts ticks (60 = 1 sec)
    const uint16_t ANIMATION_DURATION = 20 * 60; // 20 seconds = 1200 ticks @ 60 FPS
    bool animationActive = false;

    // === Indicator blink timer ===
    uint16_t indicatorTimer = 0;
    const uint16_t INDICATOR_CYCLE_TICKS = 6 * 60;  // 6 seconds per cycle (at 60 FPS)
    const uint16_t INDICATOR_TOTAL_TICKS = 20 * 60; // 20 seconds total
    const uint16_t TICKS_PER_PHASE = 2 * 60;

    // === DIPPER blink timer ===
    bool dipperOn = false;
    uint16_t dipperFlashTimer = 0;
    uint8_t  dipperFlashCount = 0;
    const uint16_t DIPPER_FLASH_DURATION = 30;

    enum class IndicatorState
    {
        LeftOnly,
        RightOnly,
        BothOn
    };
    IndicatorState indicatorState = IndicatorState::LeftOnly;

    // For temperature 21 ↔ 25
    uint16_t tempOscTimer = 0;

    // Fuel animation timer
    uint16_t fuelTimer = 0;
    enum class FuelState { Full, Fuel80, Fuel50, Low, Done };
    FuelState fuelState = FuelState::Full;

    // === Cluster frame timer ===
    enum class Phase
    {
        Forward1,
        Reverse1,
        Forward2,
        Reverse2,
        Forward3
    };
    Phase phase = Phase::Forward1;

    static const uint16_t LEFT_ANIM_TOTAL_FRAMES = 40; // 000 to 039
    static const uint8_t LEFT_ANIM_SPEED = 6;

    static const uint16_t RIGHT_ANIM_TOTAL_FRAMES = 40; // 000 to 039
    static const uint8_t RIGHT_ANIM_SPEED = 6;

    uint32_t animationStartTick = 0;
    const uint32_t ANIMATION_DURATION_TICKS = 20 * 60;

};

#endif // CLUSTERVIEW_HPP
