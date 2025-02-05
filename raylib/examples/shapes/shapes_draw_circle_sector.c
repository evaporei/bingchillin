/*******************************************************************************************
*
*   raylib [shapes] example - draw circle sector (with gui options)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - draw circle sector");

    rlVector2 center = {(rlGetScreenWidth() - 300)/2.0f, rlGetScreenHeight()/2.0f };

    float outerRadius = 180.0f;
    float startAngle = 0.0f;
    float endAngle = 180.0f;
    float segments = 10.0f;
    float minSegments = 4;

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

            rlDrawCircleSector(center, outerRadius, startAngle, endAngle, (int)segments, rlFade(MAROON, 0.3f));
            rlDrawCircleSectorLines(center, outerRadius, startAngle, endAngle, (int)segments, rlFade(MAROON, 0.6f));

            // Draw GUI controls
            //------------------------------------------------------------------------------
            GuiSliderBar((rlRectangle){ 600, 40, 120, 20}, "StartAngle", rlTextFormat("%.2f", startAngle), &startAngle, 0, 720);
            GuiSliderBar((rlRectangle){ 600, 70, 120, 20}, "EndAngle", rlTextFormat("%.2f", endAngle), &endAngle, 0, 720);

            GuiSliderBar((rlRectangle){ 600, 140, 120, 20}, "Radius", rlTextFormat("%.2f", outerRadius), &outerRadius, 0, 200);
            GuiSliderBar((rlRectangle){ 600, 170, 120, 20}, "Segments", rlTextFormat("%.2f", segments), &segments, 0, 100);
            //------------------------------------------------------------------------------

            minSegments = truncf(ceilf((endAngle - startAngle) / 90));
            rlDrawText(rlTextFormat("MODE: %s", (segments >= minSegments)? "MANUAL" : "AUTO"), 600, 200, 10, (segments >= minSegments)? MAROON : DARKGRAY);

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