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

    // Ensure scrollable container is initialized at the top
    scrollableContainer1.doScroll(0, 0);
    scrollableContainer1.invalidate();

    // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
//    tickCounter = 0;
}

void Text_Scroll_demoView::tearDownScreen()
{
    Text_Scroll_demoViewBase::tearDownScreen();
}

/*
void Text_Scroll_demoView::handleTickEvent()
{
    static int32_t scrollPosition = 0; // Current scroll offset (Y)
    static bool scrollingDown = true; // Direction: true = down, false = up
    static bool completed = false; // Track if full cycle is done
    const int32_t CONTENT_HEIGHT = 1548; // Height of textArea1
    const int32_t VIEWPORT_HEIGHT = 406; // Height of scrollableContainer1
    const int32_t MAX_SCROLL = CONTENT_HEIGHT - VIEWPORT_HEIGHT; // 1142
    const int32_t SCROLL_SPEED = 5; // Pixels per tick (~60 pixels/sec at 60 FPS)

    if (completed) {
        return; // Stop scrolling after one full cycle
    }

    // Update scroll position and apply delta
    int32_t scrollDelta = scrollingDown ? -SCROLL_SPEED : SCROLL_SPEED;
    scrollPosition += scrollingDown ? SCROLL_SPEED : -SCROLL_SPEED;

    // Check boundaries and switch direction
    if (scrollingDown && scrollPosition >= MAX_SCROLL) {
        scrollPosition = MAX_SCROLL; // Clamp to max
        scrollingDown = false; // Switch to scrolling up
    } else if (!scrollingDown && scrollPosition <= 0) {
        scrollPosition = 0; // Clamp to min
        completed = true; // Mark cycle as complete
    }

    // Apply scroll delta
    scrollableContainer1.doScroll(0, scrollDelta);
    scrollableContainer1.invalidate();
}
*/

void Text_Scroll_demoView::handleTickEvent()
{
    static float scrollPosition = 0;                             // Current scroll offset (Y)
    static bool scrollingDown = true;                            // Direction: true = down, false = up
    static uint32_t tickCounter = 0;                             // Track total ticks
    const int32_t CONTENT_HEIGHT = 1548;                         // Height of textArea1
    const int32_t VIEWPORT_HEIGHT = 406;                         // Height of scrollableContainer1
    const int32_t MAX_SCROLL = CONTENT_HEIGHT - VIEWPORT_HEIGHT; // 1142
    const float SCROLL_SPEED = 4;                                // 4 pixels/tick (~240 pixels/sec)
    const uint32_t TOTAL_DURATION_TICKS = 600;                   // 10 seconds at 60 FPS

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

        tickCounter++; // Increment tick counter

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