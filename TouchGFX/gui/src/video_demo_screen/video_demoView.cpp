#include <gui/video_demo_screen/video_demoView.hpp>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

video_demoView::video_demoView() : tickCounter(0)
{

}

void video_demoView::setupScreen()
{
    video_demoViewBase::setupScreen();

     // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
    tickCounter = 0;
}

void video_demoView::tearDownScreen()
{
    video_demoViewBase::tearDownScreen();
}

void video_demoView::handleTickEvent()
{
    if (tickCounter < TOTAL_DURATION_TICKS)
    {
        tickCounter++;
        if(demo_running)
        {
             updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
        }
    }
    else
    {
        // After 10 seconds, change screen
        demo_complete = false;
        test_name = 1;
        application().gotoResult_screenScreenNoTransition();
    }
}

void video_demoView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
