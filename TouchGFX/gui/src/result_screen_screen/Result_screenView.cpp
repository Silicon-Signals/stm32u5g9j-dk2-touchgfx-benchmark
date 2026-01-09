#include <gui/result_screen_screen/Result_screenView.hpp>
#include "main.h"

Result_screenView::Result_screenView()
{
    
}

void Result_screenView::setupScreen()
{
    Result_screenViewBase::setupScreen();

    updateTextAreas(avg_fps, avg_ram_usage, avg_internal_flash,  avg_external_flash, avg_render_time, avg_cpu_usage);
    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);

    application().autoStartDemo = true;
}

void Result_screenView::tearDownScreen()
{
    Result_screenViewBase::tearDownScreen();
}

void Result_screenView::handleTickEvent()
{
    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
}

void Result_screenView::updateTextAreas(int fps, int ram_usage, int internal_flash, int external_flash, int renderTime, int cpu)
{
    // Hide all Test name by default
    Video_test_name.setVisible(false);
    Video_test_name.invalidate();
    Image_test_name.setVisible(false);
    Image_test_name.invalidate();
    Static_test_name.setVisible(false);
    Static_test_name.invalidate();
    SVG_test_name.setVisible(false);
    SVG_test_name.invalidate();
    Text_scroll_test_name.setVisible(false);
    Text_scroll_test_name.invalidate();
    Cluster_test_name.setVisible(false);
    Cluster_test_name.invalidate();
    

    // Show the selected Test based on test_name
    if (test_name == 1) {
        Video_test_name.setVisible(true);
        Video_test_name.invalidate();
    } else if (test_name == 2) {
        Image_test_name.setVisible(true);
        Image_test_name.invalidate();
    } else if (test_name == 3) {
        Static_test_name.setVisible(true);
        Static_test_name.invalidate();
    } else if (test_name == 4) {
        SVG_test_name.setVisible(true);
        SVG_test_name.invalidate();
    } else if (test_name == 5) {
        Text_scroll_test_name.setVisible(true);
        Text_scroll_test_name.invalidate();
    } else if (test_name == 6) {
        Cluster_test_name.setVisible(true);
        Cluster_test_name.invalidate();
    }

    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(RAM_usageBuffer, 10, "%d", ram_usage);
    RAM_usage.invalidate();

    Unicode::snprintf(Internal_FlashBuffer, 10, "%d", internal_flash);
    Internal_Flash.invalidate();

    Unicode::snprintf(External_FlashBuffer, 10, "%d", external_flash);
    External_Flash.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
    
    Unicode::snprintf(CPUBuffer, 10, "%d", cpu);
    CPU.invalidate(); 
}

void Result_screenView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPS_runtimeBuffer, 10, "%d", fps);
    FPS_runtime.invalidate();

    Unicode::snprintf(CPU_usage_runtimeBuffer, 10, "%d", cpu);
    CPU_usage_runtime.invalidate();

    Unicode::snprintf(Render_time_runtimeBuffer, 10, "%d", renderTime);
    Render_time_runtime.invalidate();
}
