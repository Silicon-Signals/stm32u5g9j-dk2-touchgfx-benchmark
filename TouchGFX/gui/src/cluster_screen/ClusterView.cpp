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
    tickCounter = 0;
    indicatorState = IndicatorState::LeftOnly;

    dipper_flashtimer = 0;
    dipper_flashcount = 0;

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
	animation_frame = 0;
	animation_tickcounter = 0;
	left_animation_running = true;
	right_animation_running = true;
    phase = Phase::Forward1;

    Left_c_cluster.setBitmap(Bitmap(BITMAP_LEFT_FRAME_000_ID));
    Right_c_cluster.setBitmap(Bitmap(BITMAP_RIGHT_FRAME_000_ID));
    Left_c_cluster.invalidate();
    Right_c_cluster.invalidate();
}

void ClusterView::handleTickEvent()
{
    if (!left_animation_running || !right_animation_running)
        return;

    tickCounter++;

    // END OF 20-SECOND DEMO
    if (tickCounter >= TOTAL_TICKS)
    {
    	left_animation_running = right_animation_running = false;

        left_indicator.setVisible(false);
        right_indicator.setVisible(false);
        left_indicator.invalidate();
        right_indicator.invalidate();

        demo_complete = false;
        test_name = 6;
        application().gotoResult_screenScreenNoTransition();
        return;
    }

    const uint16_t seconds = tickCounter / 60;
    const uint16_t cyclePhase = (tickCounter % INDICATOR_CYCLE_TICKS) / TICKS_PER_PHASE;

    // TURN INDICATORS (6-second cycle)
    switch (cyclePhase)
    {
    case 0:
        left_indicator.setVisible(true);
        right_indicator.setVisible(false);
        break;
    case 1:
        left_indicator.setVisible(false);
        right_indicator.setVisible(true);
        break;
    default:
        left_indicator.setVisible(true);
        right_indicator.setVisible(true);
        break;
    }
    left_indicator.invalidate();
    right_indicator.invalidate();

    // DIPPER FLASH at 8s and 15s
    if ((seconds == 8 || seconds == 15) && dipper_flashcount == 0)
    {
    	dipper_flashtimer = 1;
    	dipper_flashcount = 1;
        dipper.setVisible(true);
        dipper.invalidate();
    }

    if (dipper_flashtimer > 0)
    {
        if (++dipper_flashtimer > DIPPER_FLASH_DURATION)
        {
            dipper.setVisible(!dipper.isVisible());
            dipper.invalidate();

            if (!dipper.isVisible() && ++dipper_flashcount >= 4)
            	dipper_flashtimer = dipper_flashcount = 0;
            else
            	dipper_flashtimer = 1;
        }
    }

    // WARNING INDICATION LOGIC (Battery, Service, Engine, Fuel)
    switch (seconds)
    {
    case 5:
        Battery_full.setVisible(false);
        Battery_50.setVisible(true);
        Battery_50.invalidate();
        break;
    case 9:
        Battery_50.setVisible(false);
        Battery_low.setVisible(true);
        low_battery_container.setVisible(true);
        Battery_low.invalidate();
        low_battery_container.invalidate();
        break;
    case 10:
        service.setVisible(true);
        white_fuel_icon.setVisible(false);
        battery_icon.setVisible(false);
        low_battery_container.setVisible(false);
        service.invalidate();
	white_fuel_icon.invalidate();
        low_battery_container.invalidate();
        break;
    case 13:
        full_fuel.setVisible(false);
        fuel_80.setVisible(true);
        fuel_80.invalidate();
        break;
    case 15:
        engine.setVisible(true);
        engine.invalidate();
        break;
    case 16:
        fuel_80.setVisible(false);
        fuel_50.setVisible(true);
        fuel_50.invalidate();
        break;
    case 19:
        fuel_50.setVisible(false);
        low_fuel.setVisible(true);
        low_fuel_container.setVisible(true);
        low_fuel.invalidate();
        low_fuel_container.invalidate();
        break;
    }

    // ANIMATION FRAME UPDATE
    if (++animation_tickcounter >= ANIM_SPEED)
    {
    	animation_tickcounter = 0;

        switch (phase)
        {
        case Phase::Forward1:
            if (++animation_frame >= ANIM_TOTAL_FRAMES - 1)
                phase = Phase::Reverse1;
            break;
        case Phase::Reverse1:
            if (--animation_frame <= 13)
                phase = Phase::Forward2;
            break;
        case Phase::Forward2:
            if (++animation_frame >= 30)
                phase = Phase::Reverse2;
            break;
        case Phase::Reverse2:
            if (--animation_frame <= 3)
                phase = Phase::Forward3;
            break;
        case Phase::Forward3:
            if (++animation_frame >= 21)
                phase = Phase::Forward1;
            break;
        }

        // CALCULATE KM/H & RPM %
        int KMH_value = 0;
        int gear_value = 0;

        if (animation_frame > 0 && animation_frame <= 4)
        {
        	gear_value = 1;
        	KMH_value = 3 + (animation_frame - 1) * 7 / 3;
        }
        else if (animation_frame > 4 && animation_frame <= 10)
        {
        	gear_value = 2;
            KMH_value = 10 + (animation_frame - 4) * 20 / 6;
        }
        else if (animation_frame > 10 && animation_frame <= 20)
        {
        	gear_value = 3;
            KMH_value = 30 + (animation_frame - 10) * 20 / 10;
        }
        else if (animation_frame > 20 && animation_frame <= 30)
        {
        	gear_value = 4;
            KMH_value = 50 + (animation_frame - 20) * 30 / 10;
        }
        else if (animation_frame > 30 && animation_frame <= 39)
        {
        	gear_value = 5;
            KMH_value = 80 + (animation_frame - 30) * 40 / 9;
        }

        const int RMP_percent = (KMH_value * 100) / 120;
        const int RMP_frame = (RMP_percent * 39) / 100;

        Left_c_cluster.setBitmap(Bitmap(BITMAP_LEFT_FRAME_000_ID + animation_frame));
        Right_c_cluster.setBitmap(Bitmap(BITMAP_RIGHT_FRAME_000_ID + RMP_frame));
        Left_c_cluster.invalidate();
        Right_c_cluster.invalidate();

        Unicode::snprintf(gearBuffer, GEAR_SIZE, "%d", gear_value);
        Unicode::snprintf(km_per_hourBuffer, KM_PER_HOUR_SIZE, "%d", KMH_value);
        Unicode::snprintf(RPMBuffer, RPM_SIZE, "%d", RMP_percent);

        gear.invalidate(); 
        km_per_hour.invalidate();
        RPM.invalidate();
    }

    // TOTAL KM (0 → 200)
    const int totalKM = (tickCounter * 201) / 1200;
    Unicode::snprintf(total_kmBuffer, TOTAL_KM_SIZE, "%d", totalKM);
    total_km.invalidate();

    if (demo_running)
        updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
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
