#include <gui/image_demo_screen/image_demoView.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <algorithm>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

image_demoView::image_demoView() : 
zAngle2D(0.0f), deltaZangle2D(0.019f)
{
    
}

void image_demoView::setupScreen()
{
    image_demoViewBase::setupScreen();

    // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
//    tickCounter = 0;

}

void image_demoView::tearDownScreen()
{
    image_demoViewBase::tearDownScreen();
}

void image_demoView::handleTickEvent()
{
    static uint32_t tickCounter = 0;
    static int16_t posX[3] = {65, 338, 601}; // Initial X positions
    static int16_t posY[3] = {280, 280, 280}; // Initial Y positions
    const uint32_t TOTAL_DURATION_TICKS = 600;
    const uint32_t FRAMES_PER_MOVE = 60;
    const uint32_t TOTAL_FRAMES = FRAMES_PER_MOVE * 8;

    // Reset animation
    if (tickCounter == 0) {
        posX[0] = 65; posY[0] = 280;
        posX[1] = 338; posY[1] = 280;
        posX[2] = 601; posY[2] = 280;
        Img_move1.setXY(posX[0], posY[0]);
        Img_move2.setXY(posX[1], posY[1]);
        Img_move3.setXY(posX[2], posY[2]);
        Img_move1.invalidate();
        Img_move2.invalidate();
        Img_move3.invalidate();
    }

    if (tickCounter < TOTAL_DURATION_TICKS) {
        uint32_t frame = tickCounter % TOTAL_FRAMES;
        uint32_t phase = frame / FRAMES_PER_MOVE;
        frame %= FRAMES_PER_MOVE;
        int16_t targetX[3] = {posX[0], posX[1], posX[2]};
        int16_t targetY[3] = {posY[0], posY[1], posY[2]};

        // Animation phases
        float offset = (60.0f * frame) / FRAMES_PER_MOVE;
        if (phase == 0) { // Up (Y: 280 → 220)
            for (int i = 0; i < 3; i++) targetY[i] = 280 - static_cast<int16_t>(offset);
        } else if (phase == 1) { // Return Y (220 → 280)
            for (int i = 0; i < 3; i++) targetY[i] = 220 + static_cast<int16_t>(offset);
        } else if (phase == 2) { // Down (Y: 280 → 340)
            for (int i = 0; i < 3; i++) targetY[i] = 280 + static_cast<int16_t>(offset);
        } else if (phase == 3) { // Return Y (340 → 280)
            for (int i = 0; i < 3; i++) targetY[i] = 340 - static_cast<int16_t>(offset);
        } else if (phase == 4) { // Left
            targetX[0] = 65 - static_cast<int16_t>(offset);
            targetX[1] = 338 - static_cast<int16_t>(offset);
            targetX[2] = 601 - static_cast<int16_t>(offset);
        } else if (phase == 5) { // Return X
            targetX[0] = 5 + static_cast<int16_t>(offset);
            targetX[1] = 278 + static_cast<int16_t>(offset);
            targetX[2] = 541 + static_cast<int16_t>(offset);
        } else if (phase == 6) { // Right
            targetX[0] = 65 + static_cast<int16_t>(offset);
            targetX[1] = 338 + static_cast<int16_t>(offset);
            targetX[2] = 601 + static_cast<int16_t>(offset);
        } else { // Return X
            targetX[0] = 125 - static_cast<int16_t>(offset);
            targetX[1] = 398 - static_cast<int16_t>(offset);
            targetX[2] = 661 - static_cast<int16_t>(offset);
        }

        // Update positions
        Img_move1.setXY(targetX[0], targetY[0]);
        Img_move2.setXY(targetX[1], targetY[1]);
        Img_move3.setXY(targetX[2], targetY[2]);
        Img_move1.invalidate();
        Img_move2.invalidate();
        Img_move3.invalidate();

        // Clear previous positions
        for (int i = 0; i < 3; i++) {
            touchgfx::Rect clearArea(
                std::min(posX[i], targetX[i]) - 5,
                std::min(posY[i], targetY[i]) - 5,
                std::abs(targetX[i] - posX[i]) + (i == 0 ? Img_move1.getWidth() : i == 1 ? Img_move2.getWidth() : Img_move3.getWidth()) + 10,
                std::abs(targetY[i] - posY[i]) + (i == 0 ? Img_move1.getHeight() : i == 1 ? Img_move2.getHeight() : Img_move3.getHeight()) + 10
            );
            invalidateRect(clearArea);
        }

        // Store positions
        for (int i = 0; i < 3; i++) {
            posX[i] = targetX[i];
            posY[i] = targetY[i];
        }

        // Update rotation
        zAngle2D += deltaZangle2D;
        Img_rotate1.updateAngles(0.0F, 0.0F, zAngle2D);
        Img_rotate2.updateAngles(0.0F, 0.0F, zAngle2D);
        Img_rotate3.updateAngles(0.0F, 0.0F, zAngle2D);

        tickCounter++;

        if(demo_running)
        {
             updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
        }
    } else {
        // Reset positions
        posX[0] = 65; posY[0] = 280;
        posX[1] = 338; posY[1] = 280;
        posX[2] = 601; posY[2] = 280;
        Img_move1.setXY(posX[0], posY[0]);
        Img_move2.setXY(posX[1], posY[1]);
        Img_move3.setXY(posX[2], posY[2]);
        Img_move1.invalidate();
        Img_move2.invalidate();
        Img_move3.invalidate();
        tickCounter = 0;
        demo_complete = false;
        test_name = 2;
        application().gotoResult_screenScreenNoTransition();
    }
}

void image_demoView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
