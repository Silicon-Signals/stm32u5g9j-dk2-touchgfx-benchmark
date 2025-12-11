#include <gui/static_demo_screen/static_demoView.hpp>
#include <touchgfx/Color.hpp>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

static_demoView::static_demoView() : tickCounter(0)
{
   
}

void static_demoView::setupScreen()
{
    static_demoViewBase::setupScreen();

    rectangle.setPosition(466, 77, 200, 100); // place at 200,100, size 200x100
    rectangle.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
    rectangle.setCornerRadius(20);
    rectangle.setAlpha(255); // Or less for making it blend
    add(rectangle);

    // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
    tickCounter = 0;
}

void static_demoView::tearDownScreen()
{
    static_demoViewBase::tearDownScreen();
}

void static_demoView::handleTickEvent()
{
    if (tickCounter < TOTAL_DURATION_TICKS)
    {
        tickCounter++;

        // Change colors every ~200ms (12 ticks at 60 FPS)
        if (tickCounter % TICKS_PER_COLOR_CHANGE == 0)
        {
            int patternIndex = (tickCounter / TICKS_PER_COLOR_CHANGE) % 3;

            touchgfx::colortype square_color, circle_color, rectangle_color;

            switch (patternIndex)
            {
            case 0:  // Red square, Green circle, Blue triangle
                square_color = touchgfx::Color::getColorFromRGB(255, 0, 0);
                circle_color = touchgfx::Color::getColorFromRGB(0, 255, 0);
                rectangle_color = touchgfx::Color::getColorFromRGB(0, 0, 255);
                break;
            case 1:  // Blue square, Red circle, Green triangle
                square_color = touchgfx::Color::getColorFromRGB(0, 0, 255);
                circle_color = touchgfx::Color::getColorFromRGB(255, 0, 0);
                rectangle_color = touchgfx::Color::getColorFromRGB(0, 255, 0);
                break;
            case 2:  // Green square, Blue circle, Red triangle
                square_color = touchgfx::Color::getColorFromRGB(0, 255, 0);
                circle_color = touchgfx::Color::getColorFromRGB(0, 0, 255);
                rectangle_color = touchgfx::Color::getColorFromRGB(255, 0, 0);
                break;
            }

            // Update square
            square.setColor(square_color);
            square.invalidate();
            alpha_square.setColor(square_color);
            alpha_square.invalidate();

            // Update rectangle
            rectangle.setColor(rectangle_color);
            rectangle.invalidate();

            // Update circle
            circlePainter.setColor(circle_color);
            circle.invalidate();
            alpha_circlePainter.setColor(circle_color);
            alpha_circle.invalidate();
        }

        if(demo_running)
        {
             updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
        }
    }
    else
    {
        // After 10 seconds, change screen
    	demo_complete = false;
        test_name = 3;
    	application().gotoResult_screenScreenNoTransition();
    }
}

void static_demoView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}

