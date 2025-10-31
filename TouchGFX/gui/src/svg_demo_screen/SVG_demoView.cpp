#include <gui/svg_demo_screen/SVG_demoView.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <cmath>
#include <algorithm>
#include "main.h"

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

SVG_demoView::SVG_demoView()
{
}

void SVG_demoView::setupScreen()
{
    SVG_demoViewBase::setupScreen();

     // Start demo sampling
    demo_running = true;
    demo_complete = false;
    sample_count = 0;
//    tickCounter = 0;
}

void SVG_demoView::tearDownScreen()
{
    SVG_demoViewBase::tearDownScreen();
}

void SVG_demoView::handleTickEvent()
{
    static uint32_t frameCount = 0;
    static bool animationRunning = false;
    const uint32_t framesPerMove = 30; // 500ms at 60 FPS
    const uint32_t delayFrames = 12;   // 200ms at 60 FPS
    const uint32_t framesPerPhase = framesPerMove + delayFrames;
    const uint32_t maxFrames = 600;    // 10 seconds at 60 FPS

    // Consolidated target positions for all phases (5 tires x 5 phases x {x, y})
    static const int16_t targetPos[5][5][2] = {
        {{188, 127}, {80, 16}, {659, 16}, {543, 127}, {367, 165}}, // Tire1
        {{367, 165}, {188, 127}, {80, 16}, {659, 16}, {543, 127}}, // Tire2
        {{543, 127}, {367, 165}, {188, 127}, {80, 16}, {659, 16}}, // Tire3
        {{659, 16}, {543, 127}, {367, 165}, {188, 127}, {80, 16}},  // Tire4
        {{80, 16}, {659, 16}, {543, 127}, {367, 165}, {188, 127}}   // Tire5
    };

    // Array of tire objects for easier iteration
    static touchgfx::Drawable* tires[5] = {&Tire1, &Tire2, &Tire3, &Tire4, &Tire5};
    static int16_t lastX[5] = {367, 543, 659, 80, 188};
    static int16_t lastY[5] = {165, 127, 16, 16, 127};

    // Array of tire_pair objects (assuming tire_pair1 to tire_pair5 exist)
    static touchgfx::Drawable* tirePairs[5] = {&tire_pair_1, &tire_pair_2, &tire_pair_3, &tire_pair_4, &tire_pair_5};

    // Initialize animation
    if (!animationRunning && frameCount == 0) {
        animationRunning = true;
        for (int i = 0; i < 5; i++) {
            lastX[i] = tires[i]->getX();
            lastY[i] = tires[i]->getY();
        //    tirePairs[i]->setVisible(false); // Ensure all tire_pairs start invisible
        }
    }

    if (animationRunning) {
        // Determine current phase (0 to 4) and cycle within 10 seconds
        uint32_t phase = (frameCount / framesPerPhase) % 5; // Cycle phases within 600 frames
        uint32_t frameInPhase = frameCount % framesPerPhase;

        if (phase < 5 && frameInPhase < framesPerMove) {
            // Animation phase
            for (int i = 0; i < 5; i++) {
                // Get start and end positions
                int16_t startX = (phase == 0 && frameCount < framesPerPhase) ? lastX[i] : targetPos[i][phase == 0 ? 4 : phase - 1][0];
                int16_t startY = (phase == 0 && frameCount < framesPerPhase) ? lastY[i] : targetPos[i][phase == 0 ? 4 : phase - 1][1];
                int16_t endX = targetPos[i][phase][0];
                int16_t endY = targetPos[i][phase][1];

                // Calculate new position using sineEaseIn with correct delta
                int16_t newX = touchgfx::EasingEquations::sineEaseIn(frameInPhase, startX, endX - startX, framesPerMove);
                int16_t newY = touchgfx::EasingEquations::sineEaseIn(frameInPhase, startY, endY - startY, framesPerMove);

                // Get tire dimensions
                int16_t tireW = tires[i]->getWidth();
                int16_t tireH = tires[i]->getHeight();

                // Calculate invalidation area covering old and new positions with margin
                int16_t minX = std::min(lastX[i], newX) - (tireW / 2 + 5);
                int16_t minY = std::min(lastY[i], newY) - (tireH / 2 + 5);
                int16_t maxX = std::max(lastX[i] + tireW, newX + tireW) + (tireW / 2 + 5);
                int16_t maxY = std::max(lastY[i] + tireH, newY + tireH) + (tireH / 2 + 5);
                touchgfx::Rect clearArea(minX, minY, maxX - minX, maxY - minY);
                invalidateRect(clearArea);

                // Update tire position
                tires[i]->setXY(newX, newY);
                tires[i]->invalidate();

                // Store new position
                lastX[i] = newX;
                lastY[i] = newY;
            }
        } else if (phase < 5 && frameInPhase == framesPerMove) {
            // Finalize positions at end of animation phase
            for (int i = 0; i < 5; i++) {
                int16_t endX = targetPos[i][phase][0];
                int16_t endY = targetPos[i][phase][1];
                tires[i]->setXY(endX, endY);

                // Invalidate final position with tire size and margin
                int16_t tireW = tires[i]->getWidth();
                int16_t tireH = tires[i]->getHeight();
                touchgfx::Rect finalArea(endX - (tireW / 2 + 5), endY - (tireH / 2 + 5), tireW + 60, tireH + 60);
                invalidateRect(finalArea);

                tires[i]->invalidate();

                // Manage tire_pair visibility for position 367x165
                if (endX == 367 && endY == 165) {
                    // Hide all tire_pairs
                    for (int j = 0; j < 5; j++) {
                        tirePairs[j]->setVisible(false);
                        tirePairs[j]->invalidate();
                    }
                    // Show the corresponding tire_pair for the current tire
                    tirePairs[i]->setVisible(true);
                    tirePairs[i]->invalidate();
                } else if (lastX[i] == 367 && lastY[i] == 165 && endX != 367) {
                    // Hide the tire_pair if the tire moves away from 367x165
                    tirePairs[i]->setVisible(false);
                    tirePairs[i]->invalidate();
                }

                lastX[i] = endX;
                lastY[i] = endY;
            }
        }

        // Increment frame count and handle 10-second transition
        frameCount++;

        if(demo_running)
        {
             updateruntime_metrics(g_fps, g_cpu_usage, g_render_time);
        }

        if (frameCount >= maxFrames) {
            animationRunning = false;
            frameCount = 0;
            demo_complete = false;
            test_name = 4;
            application().gotoResult_screenScreenNoTransition();
        }
    }
}

void SVG_demoView::updateruntime_metrics(int fps, int cpu, int renderTime)
{
    Unicode::snprintf(FPSBuffer, 10, "%d", fps);
    FPS.invalidate();

    Unicode::snprintf(CPU_usageBuffer, 10, "%d", cpu);
    CPU_usage.invalidate();

    Unicode::snprintf(Render_timeBuffer, 10, "%d", renderTime);
    Render_time.invalidate();
}
