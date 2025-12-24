#include <gui/home_screen_screen/home_screenView.hpp>
#include "main.h"

home_screenView::home_screenView()
{
}

void home_screenView::setupScreen()
{
    home_screenViewBase::setupScreen();

    start_btn.setTouchable(false);
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
    // Update runtime metrics every tick
    updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
}

void home_screenView::handleClickEvent(const touchgfx::ClickEvent &evt)
{
    home_screenViewBase::handleClickEvent(evt);

    if (evt.getType() == touchgfx::ClickEvent::RELEASED)
    {
        int16_t x = evt.getX();
        int16_t y = evt.getY();

        // Get absolute position of the start_btn on screen
        Rect btnRect = start_btn.getAbsoluteRect(); // This is the easiest and safest way

        int16_t centerX = btnRect.x + 190 / 2;
        int16_t centerY = btnRect.y + 190 / 2;
        int16_t radius = 190 / 2; // square width/2

        int16_t dx = x - centerX;
        int16_t dy = y - centerY;

        // Check if click is inside the circle
        // Check if click is inside the circle
        if (dx * dx + dy * dy <= radius * radius)
        {
            Start_text.setVisible(false);
            Start_text.invalidate();

            // display_video_demo
            video_demo.setVisible(true);
            video_demo.invalidate();

            // move_out_video_demo
            video_demo.clearMoveAnimationEndedAction();
            video_demo.startMoveAnimation(234, 313, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_image_demo
            image_demo.setVisible(true);
            image_demo.invalidate();

            // move_out_image_demo
            image_demo.clearMoveAnimationEndedAction();
            image_demo.startMoveAnimation(203, 191, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_static_demo
            static_demo.setVisible(true);
            static_demo.invalidate();

            // move_out_static_demo
            static_demo.clearMoveAnimationEndedAction();
            static_demo.startMoveAnimation(284, 64, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_SVG_demo
            SVG_demo.setVisible(true);
            SVG_demo.invalidate();

            // move_out_SVG_demo
            SVG_demo.clearMoveAnimationEndedAction();
            SVG_demo.startMoveAnimation(447, 64, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_text_scroll_demo
            Text_scroll_demo.setVisible(true);
            Text_scroll_demo.invalidate();

            // move_out_text_scroll_demo
            Text_scroll_demo.clearMoveAnimationEndedAction();
            Text_scroll_demo.startMoveAnimation(534, 191, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_close_btn
            close_btn.setVisible(true);
            close_btn.invalidate();

            // move_out_close_btn
            close_btn.clearMoveAnimationEndedAction();
            close_btn.startMoveAnimation(370, 373, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);

            // display_cluster_demo
            cluster_demo.setVisible(true);
            cluster_demo.invalidate();

            // move_out_cluster_demo
            cluster_demo.clearMoveAnimationEndedAction();
            cluster_demo.startMoveAnimation(495, 320, 48, touchgfx::EasingEquations::sineEaseIn, touchgfx::EasingEquations::sineEaseIn);
        }
    }

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
