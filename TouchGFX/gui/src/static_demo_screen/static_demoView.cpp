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
            int patternIndex = (tickCounter / TICKS_PER_COLOR_CHANGE) % 3;  // Cycle 0,1,2

            touchgfx::colortype squareColor, circleColor, rectColor;

            switch (patternIndex)
            {
            case 0:  // Red square, Green circle, Blue triangle
                squareColor = touchgfx::Color::getColorFromRGB(255, 0, 0);
                circleColor = touchgfx::Color::getColorFromRGB(0, 255, 0);
                rectColor = touchgfx::Color::getColorFromRGB(0, 0, 255);
                break;
            case 1:  // Blue square, Red circle, Green triangle
                squareColor = touchgfx::Color::getColorFromRGB(0, 0, 255);
                circleColor = touchgfx::Color::getColorFromRGB(255, 0, 0);
                rectColor = touchgfx::Color::getColorFromRGB(0, 255, 0);
                break;
            case 2:  // Green square, Blue circle, Red triangle
                squareColor = touchgfx::Color::getColorFromRGB(0, 255, 0);
                circleColor = touchgfx::Color::getColorFromRGB(0, 0, 255);
                rectColor = touchgfx::Color::getColorFromRGB(255, 0, 0);
                break;
            }

            // Update square
            square.setColor(squareColor);
            square.invalidate();
            alpha_square.setColor(squareColor);
            alpha_square.invalidate();

            // Update rectangle
            rectangle.setColor(rectColor);
            rectangle.invalidate();

            // Update circle
            circlePainter.setColor(circleColor);
            circle.invalidate();
            alpha_circlePainter.setColor(circleColor);
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

