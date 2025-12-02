#include <gui/cluster_screen/ClusterView.hpp>
#include <images/BitmapDatabase.hpp>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

ClusterView::ClusterView()
{
    Unicode::snprintf(gearBuffer, GEAR_SIZE, "1");
    Unicode::snprintf(km_per_hourBuffer, KM_PER_HOUR_SIZE, "3");
    gear.setWildcard(gearBuffer);
    km_per_hour.setWildcard(km_per_hourBuffer);
}

void ClusterView::setupScreen()
{
    ClusterViewBase::setupScreen();

    // Start with LEFT indicator ON
    left_indicator.setVisible(true);
    right_indicator.setVisible(false);
    left_indicator.invalidate();
    right_indicator.invalidate();

    dipper.setVisible(false);
    service.setVisible(false);
    engine.setVisible(false);
    dipper.invalidate();
    service.invalidate();
    engine.invalidate();

    // Reset timers
    totalTimer = 0;
    indicatorTimer = 0;
    indicatorState = IndicatorState::LeftOnly;

    dipperOn = false;
    dipperFlashTimer = 0;
    dipperFlashCount = 0;

    fuelState = FuelState::Full;
    fuelTimer = 0;

    // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;

    startBothAnimations();
}

void ClusterView::tearDownScreen()
{
    ClusterViewBase::tearDownScreen();
}

void ClusterView::startBothAnimations()
{
    leftAnimFrame = 0;
    rightAnimFrame = 0;
    leftAnimTickCounter = 0;
    rightAnimTickCounter = 0;
    leftAnimationRunning = true;
    rightAnimationRunning = true;
    animationActive = true;
    phase = Phase::Forward1;

    Left_c_cluster.setBitmap(Bitmap(BITMAP_LEFT_FRAME_000_ID));
    Right_c_cluster.setBitmap(Bitmap(BITMAP_RIGHT_FRAME_000_ID));
    Left_c_cluster.invalidate();
    Right_c_cluster.invalidate();
}

void ClusterView::handleTickEvent()
{
    if (!leftAnimationRunning || !rightAnimationRunning)
        return;

    leftAnimTickCounter++;
    rightAnimTickCounter++;

    // === 20-SECOND TOTAL TIMER ===
    if (indicatorTimer >= INDICATOR_TOTAL_TICKS)
    {
        leftAnimationRunning = false;
        rightAnimationRunning = false;

        left_indicator.setVisible(false);
        right_indicator.setVisible(false);
        left_indicator.invalidate();
        right_indicator.invalidate();

        demo_complete = false;
        test_name = 6;
        application().gotoResult_screenScreenNoTransition();

        return;
    }

    // === INDICATOR CYCLE LOGIC (every 6 seconds) ===
    uint16_t cycleTime = indicatorTimer % INDICATOR_CYCLE_TICKS;

    if (cycleTime < TICKS_PER_PHASE) // 0–2 sec
    {
        indicatorState = IndicatorState::LeftOnly;
        left_indicator.setVisible(true);
        right_indicator.setVisible(false);
    }
    else if (cycleTime < TICKS_PER_PHASE * 2) // 2–4 sec
    {
        indicatorState = IndicatorState::RightOnly;
        left_indicator.setVisible(false);
        right_indicator.setVisible(true);
    }
    else // 4–6 sec
    {
        indicatorState = IndicatorState::BothOn;
        left_indicator.setVisible(true);
        right_indicator.setVisible(true);
    }

    left_indicator.invalidate();
    right_indicator.invalidate();

    // === WARNING LIGHTS LOGIC (Dipper, Service, Engine) ===
    uint16_t seconds = indicatorTimer / 60; // convert ticks to seconds

    // --- Dipper: Flash at 8s and 15s (500ms ON) ---
    // Start flash sequence at 8s or 15s
    if ((seconds == 8 || seconds == 15) && !dipperOn && dipperFlashTimer == 0)
    {
        dipper.setVisible(true);
        dipper.invalidate();
        dipperOn = true;
        dipperFlashTimer = 1;
        dipperFlashCount = 1;
    }

    // Control 500ms flash duration
    if (dipperOn || dipperFlashCount > 0)
    {
        dipperFlashTimer++;

        if (dipperFlashTimer >= DIPPER_FLASH_DURATION)
        {
            if (dipperOn)
            {
                // Turn OFF
                dipper.setVisible(false);
                dipper.invalidate();
                dipperOn = false;
            }
            else
            {
                // Turn ON (only if less than 2 times)
                if (dipperFlashCount < 2)
                {
                    dipper.setVisible(true);
                    dipper.invalidate();
                    dipperOn = true;
                    dipperFlashCount++;
                }
                else
                {
                    // Done: 2 flashes complete
                    dipperFlashTimer = 0;
                    dipperFlashCount = 0;
                    return; // exit early
                }
            }
            dipperFlashTimer = 1; // restart timer for next phase
        }
    }

    // --- Service: ON at 10s and stays ON ---
    if (seconds >= 10)
    {
        service.setVisible(true);
        service.invalidate();
        white_fuel_icon.setVisible(false);
        white_fuel_icon.invalidate();
        battery_icon.setVisible(false);
        battery_icon.invalidate();
    }

    // --- Engine: ON at 15s and stays ON ---
    if (seconds >= 15)
    {
        engine.setVisible(true);
        engine.invalidate();
    }

    // Display Battery Bar
    if (seconds == 5)
    {
        Battery_full.setVisible(false);
        Battery_50.setVisible(true);
        Battery_low.setVisible(false);
        Battery_full.invalidate();
        Battery_50.invalidate();
    }

    else if (seconds == 9)
    {
        Battery_50.setVisible(false);
        Battery_low.setVisible(true);
        Battery_low.invalidate();
        Battery_50.invalidate();
        low_battery_container.setVisible(true);
        low_battery_container.invalidate();
    }

    else if (seconds == 10)
    {
        low_battery_container.setVisible(false);
        low_battery_container.invalidate();
    }

    // === ANIMATION FRAME UPDATE ===
    if (leftAnimTickCounter >= LEFT_ANIM_SPEED &&
        rightAnimTickCounter >= RIGHT_ANIM_SPEED)
    {
        leftAnimTickCounter = 0;
        rightAnimTickCounter = 0;

        // === LEFT CLUSTER PHASE LOGIC (unchanged) ===
        switch (phase)
        {
        case Phase::Forward1:
            leftAnimFrame++;
            if (leftAnimFrame >= LEFT_ANIM_TOTAL_FRAMES - 1)
                phase = Phase::Reverse1;
            break;
        case Phase::Reverse1:
            leftAnimFrame--;
            if (leftAnimFrame <= 13)
                phase = Phase::Forward2;
            break;
        case Phase::Forward2:
            leftAnimFrame++;
            if (leftAnimFrame >= 30)
                phase = Phase::Reverse2;
            break;
        case Phase::Reverse2:
            leftAnimFrame--;
            if (leftAnimFrame <= 3)
                phase = Phase::Forward3;
            break;
        case Phase::Forward3:
            leftAnimFrame++;
            if (leftAnimFrame >= 21)
                phase = Phase::Forward1;
            break;
        }

        // === CALCULATE KM/H & RPM % (from left frame) ===
        int currentFrame = leftAnimFrame;
        int gearValue = 0;
        int kmhValue = 0;

        if (currentFrame >= 1 && currentFrame <= 4)
        {
            gearValue = 1;
            kmhValue = 3 + (currentFrame - 1) * 7 / 3;
        }
        else if (currentFrame >= 4 && currentFrame <= 10)
        {
            gearValue = 2;
            kmhValue = 10 + (currentFrame - 4) * 20 / 6;
        }
        else if (currentFrame >= 10 && currentFrame <= 20)
        {
            gearValue = 3;
            kmhValue = 30 + (currentFrame - 10) * 20 / 10;
        }
        else if (currentFrame >= 20 && currentFrame <= 30)
        {
            gearValue = 4;
            kmhValue = 50 + (currentFrame - 20) * 30 / 10;
        }
        else if (currentFrame >= 30 && currentFrame <= 39)
        {
            gearValue = 5;
            kmhValue = 80 + (currentFrame - 30) * 40 / 9;
        }

        // === RPM % (0 to 100) ===
        const int MAX_SPEED = 120;
        int rpmPercent = (kmhValue * 100) / MAX_SPEED;
        if (rpmPercent < 0)
            rpmPercent = 0;
        if (rpmPercent > 100)
            rpmPercent = 100;

        // === RIGHT CLUSTER: MAP RPM % → FRAME 0 to 39 ===
        int rightAnimFrame = (rpmPercent * 39) / 100; // 0 to 39 integer

        // === UPDATE LEFT CLUSTER IMAGE ===
        Left_c_cluster.setBitmap(Bitmap(BITMAP_LEFT_FRAME_000_ID + leftAnimFrame));
        Left_c_cluster.invalidate();

        // === UPDATE RIGHT CLUSTER IMAGE (driven by RPM %) ===
        Right_c_cluster.setBitmap(Bitmap(BITMAP_RIGHT_FRAME_000_ID + rightAnimFrame));
        Right_c_cluster.invalidate();

        // === UPDATE TEXT WIDGETS ===
        Unicode::snprintf(gearBuffer, GEAR_SIZE, "%d", gearValue);
        Unicode::snprintf(km_per_hourBuffer, KM_PER_HOUR_SIZE, "%d", kmhValue);
        Unicode::snprintf(RPMBuffer, RPM_SIZE, "%d", rpmPercent);

        gear.invalidate();
        km_per_hour.invalidate();
        RPM.invalidate();
    }

    // === FUEL LEVEL ANIMATION (12 to 19 seconds) ===
    if (indicatorTimer >= 12 * 60 && indicatorTimer <= 19 * 60) // 12s to 19s
    {
        fuelTimer++;

        if (fuelState == FuelState::Full)
        {
            full_fuel.setVisible(false);
            fuel_80.setVisible(true);
            fuel_80.invalidate();
            full_fuel.invalidate();
            fuelState = FuelState::Fuel80;
            fuelTimer = 0;
        }
        else if (fuelState == FuelState::Fuel80 && fuelTimer >= 180) // +3 sec
        {
            fuel_80.setVisible(false);
            fuel_50.setVisible(true);
            fuel_50.invalidate();
            fuel_80.invalidate();
            fuelState = FuelState::Fuel50;
            fuelTimer = 0;
        }
        else if (fuelState == FuelState::Fuel50 && fuelTimer >= 180) // +3 sec
        {
            fuel_50.setVisible(false);
            low_fuel.setVisible(true);
            low_fuel.invalidate();
            fuel_50.invalidate();
            fuelState = FuelState::Low;
            fuelTimer = 0;
            low_fuel_container.setVisible(true);
            low_fuel_container.invalidate();
        }
    }

    // === TOTAL KM: 0 → 200 km in 20 seconds ===
    int currentKm = (indicatorTimer * 201) / 1200;
    Unicode::snprintf(total_kmBuffer, TOTAL_KM_SIZE, "%d", currentKm);
    total_km.invalidate();

    if (demo_running)
    {
        updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
    }

    // === INCREMENT TIMER ===
    indicatorTimer++;
}

void ClusterView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
