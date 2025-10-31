#include <gui/image_demo_old_screen/image_demo_oldView.hpp>

extern volatile bool demo_running;
extern volatile bool demo_complete;
extern volatile uint32_t sample_count;

image_demo_oldView::image_demo_oldView() :
tickCounter(0), currentPositionIndex(0)
{
    // Define different positions
    positions[0] = {635, 11};
    positions[1] = {635, 111};
    positions[2] = {635, 211};
    positions[3] = {635, 311};
    positions[4] = {635, 411};
    positions[5] = {535, 411};
    positions[6] = {435, 411};
    positions[7] = {335, 411};
    positions[8] = {235, 411};
    positions[9] = {135, 411};
    positions[10] = {35, 411};
    positions[11] = {135, 311};
    positions[12] = {235, 211};
    positions[13] = {335, 111};
    positions[14] = {435, 11};
    positions[15] = {535, 11};
}

void image_demo_oldView::setupScreen()
{
    image_demo_oldViewBase::setupScreen();

       // Start demo sampling
//    demo_running = true;
//    demo_complete = false;
//    sample_count = 0;
//    tickCounter = 0;

    // Start displaying at first position
    si_logo.setXY(positions[0].x, positions[0].y);
    si_logo.setVisible(true);
    si_logo.invalidate();
}

void image_demo_oldView::tearDownScreen()
{
    image_demo_oldViewBase::tearDownScreen();
}

void image_demo_oldView::handleTickEvent()
{
    if (tickCounter < TOTAL_DURATION_TICKS)
    {
        tickCounter++;

        // Change position every ~500ms (30 ticks at 60 FPS)
        if (tickCounter % TICKS_PER_MOVE == 0 && currentPositionIndex < POSITIONS_COUNT - 1)
        {
            // Hide the image at the old position
            si_logo.setVisible(false);
            si_logo.invalidate();

            // Move to the next position and make visible
            currentPositionIndex++;
            si_logo.setXY(positions[currentPositionIndex].x, positions[currentPositionIndex].y);
            si_logo.setVisible(true);
            si_logo.invalidate();
        }
        else if (tickCounter % TICKS_PER_MOVE == 0 && currentPositionIndex == POSITIONS_COUNT - 1)
        {
            // Loop back to start after last position
            si_logo.setVisible(false);
            si_logo.invalidate();
            currentPositionIndex = 0;
            si_logo.setXY(positions[currentPositionIndex].x, positions[currentPositionIndex].y);
            si_logo.setVisible(true);
            si_logo.invalidate();
        }
    }
    else
    {
        // After 10 seconds, hide the image
        si_logo.setVisible(false);
        si_logo.invalidate();
//        demo_complete = false;
        application().gotoResult_screenScreenNoTransition();
    }
}
