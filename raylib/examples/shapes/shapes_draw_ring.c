/*******************************************************************************************
*
*   raylib [shapes] example - draw ring (with gui options)
*
*   Example originally created with raylib 2.5, last time updated with raylib 2.5
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2018-2024 Vlad Adrian (@demizdor) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - draw ring");

    rlVector2 center = {(rlGetScreenWidth() - 300)/2.0f, rlGetScreenHeight()/2.0f };

    float innerRadius = 80.0f;
    float outerRadius = 190.0f;

    float startAngle = 0.0f;
    float endAngle = 360.0f;
    float segments = 0.0f;

    bool drawRing = true;
    bool drawRingLines = false;
    bool drawCircleLines = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // NOTE: All variables update happens inside GUI control functions
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawLine(500, 0, 500, rlGetScreenHeight(), rlFade(LIGHTGRAY, 0.6f));
            rlDrawRectangle(500, 0, rlGetScreenWidth() - 500, rlGetScreenHeight(), rlFade(LIGHTGRAY, 0.3f));

            if (drawRing) rlDrawRing(center, innerRadius, outerRadius, startAngle, endAngle, (int)segments, rlFade(MAROON, 0.3f));
            if (drawRingLines) rlDrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, (int)segments, rlFade(BLACK, 0.4f));
            if (drawCircleLines) rlDrawCircleSectorLines(center, outerRadius, startAngle, endAngle, (int)segments, rlFade(BLACK, 0.4f));

            // Draw GUI controls
            //------------------------------------------------------------------------------
            GuiSliderBar((rlRectangle){ 600, 40, 120, 20 }, "StartAngle", rlTextFormat("%.2f", startAngle), &startAngle, -450, 450);
            GuiSliderBar((rlRectangle){ 600, 70, 120, 20 }, "EndAngle", rlTextFormat("%.2f", endAngle), &endAngle, -450, 450);

            GuiSliderBar((rlRectangle){ 600, 140, 120, 20 }, "InnerRadius", rlTextFormat("%.2f", innerRadius), &innerRadius, 0, 100);
            GuiSliderBar((rlRectangle){ 600, 170, 120, 20 }, "OuterRadius", rlTextFormat("%.2f", outerRadius), &outerRadius, 0, 200);

            GuiSliderBar((rlRectangle){ 600, 240, 120, 20 }, "Segments", rlTextFormat("%.2f", segments), &segments, 0, 100);

            GuiCheckBox((rlRectangle){ 600, 320, 20, 20 }, "Draw Ring", &drawRing);
            GuiCheckBox((rlRectangle){ 600, 350, 20, 20 }, "Draw RingLines", &drawRingLines);
            GuiCheckBox((rlRectangle){ 600, 380, 20, 20 }, "Draw CircleLines", &drawCircleLines);
            //------------------------------------------------------------------------------

            int minSegments = (int)ceilf((endAngle - startAngle)/90);
            rlDrawText(rlTextFormat("MODE: %s", (segments >= minSegments)? "MANUAL" : "AUTO"), 600, 270, 10, (segments >= minSegments)? MAROON : DARKGRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}