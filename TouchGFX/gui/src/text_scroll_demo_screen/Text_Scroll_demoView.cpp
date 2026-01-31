#include <gui/text_scroll_demo_screen/Text_Scroll_demoView.hpp>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

Text_Scroll_demoView::Text_Scroll_demoView()
{
}

void Text_Scroll_demoView::setupScreen()
{
    Text_Scroll_demoViewBase::setupScreen();

    // Reset all state
    scrollPosition = 0.0f;
	scrollingDown = true;
	tickCounter = 0;

    // Ensure scrollable container is initialized at the top
    scrollableContainer1.doScroll(0, 0);
    scrollableContainer1.invalidate();

    // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
}

void Text_Scroll_demoView::tearDownScreen()
{
    Text_Scroll_demoViewBase::tearDownScreen();
}

void Text_Scroll_demoView::handleTickEvent()
{
    const int32_t CONTENT_HEIGHT = 1548;
    const int32_t VIEWPORT_HEIGHT = 406;
    const int32_t MAX_SCROLL = CONTENT_HEIGHT - VIEWPORT_HEIGHT;
    const float SCROLL_SPEED = 4;
    const uint32_t TOTAL_DURATION_TICKS = 600;

    if (tickCounter < TOTAL_DURATION_TICKS)
    {
        // Update scroll position and apply delta
        float scrollDelta = scrollingDown ? -SCROLL_SPEED : SCROLL_SPEED;
        scrollPosition += scrollingDown ? SCROLL_SPEED : -SCROLL_SPEED;

        // Check boundaries and switch direction
        if (scrollingDown && scrollPosition >= MAX_SCROLL)
        {
            scrollPosition = MAX_SCROLL; // Clamp to max
            scrollingDown = false;       // Switch to scrolling up
        }
        else if (!scrollingDown && scrollPosition <= 0)
        {
            scrollPosition = 0; // Clamp to min
            scrollingDown = true; // Switch to scrolling down
        }

        // Apply scroll delta
        scrollableContainer1.doScroll(0, static_cast<int32_t>(scrollDelta));
        scrollableContainer1.invalidate();

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
        tickCounter = 0;
        test_name = 5;
        application().gotoResult_screenScreenNoTransition();
        return;
    }
}

void Text_Scroll_demoView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
