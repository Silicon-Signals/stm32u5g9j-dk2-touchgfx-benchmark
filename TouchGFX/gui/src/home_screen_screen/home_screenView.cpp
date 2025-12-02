#include <gui/home_screen_screen/home_screenView.hpp>
#include "main.h"

home_screenView::home_screenView()
{
}

void home_screenView::setupScreen()
{
    home_screenViewBase::setupScreen();

    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);

    if (application().autoStartDemo)
    {
        application().autoStartDemo = false; // reset flag

        // AUTO PRESS THE START BUTTON!
        runStartButtonAnimation();
    }
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

void home_screenView::runStartButtonAnimation()
{
    Start_text.setVisible(false);
    Start_text.invalidate();

    video_demo.setVisible(true);
    video_demo.invalidate();

    video_demo.clearMoveAnimationEndedAction();
    video_demo.startMoveAnimation(234, 313, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    image_demo.setVisible(true);
    image_demo.invalidate();

    image_demo.clearMoveAnimationEndedAction();
    image_demo.startMoveAnimation(203, 191, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    static_demo.setVisible(true);
    static_demo.invalidate();

    static_demo.clearMoveAnimationEndedAction();
    static_demo.startMoveAnimation(284, 64, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    SVG_demo.setVisible(true);
    SVG_demo.invalidate();

    SVG_demo.clearMoveAnimationEndedAction();
    SVG_demo.startMoveAnimation(447, 64, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    Text_scroll_demo.setVisible(true);
    Text_scroll_demo.invalidate();

    Text_scroll_demo.clearMoveAnimationEndedAction();
    Text_scroll_demo.startMoveAnimation(534, 191, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    close_btn.setVisible(true);
    close_btn.invalidate();

    close_btn.clearMoveAnimationEndedAction();
    close_btn.startMoveAnimation(370, 373, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

    cluster_demo.setVisible(true);
    cluster_demo.invalidate();

    cluster_demo.clearMoveAnimationEndedAction();
    cluster_demo.startMoveAnimation(495, 320, 1, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);
}
