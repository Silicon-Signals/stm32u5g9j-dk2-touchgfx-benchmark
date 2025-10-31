#include <gui/home_screen_screen/home_screenView.hpp>
#include "main.h"

home_screenView::home_screenView()
{

}

void home_screenView::setupScreen()
{
    home_screenViewBase::setupScreen();

    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
}

void home_screenView::tearDownScreen()
{
    home_screenViewBase::tearDownScreen();
}

void home_screenView::handleTickEvent()
{
    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
}

void home_screenView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
