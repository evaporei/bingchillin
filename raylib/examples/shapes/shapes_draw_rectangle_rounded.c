/*******************************************************************************************
*
*   raylib [shapes] example - draw rectangle rounded (with gui options)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - draw rectangle rounded");

    float roundness = 0.2f;
    float width = 200.0f;
    float height = 100.0f;
    float segments = 0.0f;
    float lineThick = 1.0f;

    bool drawRect = false;
    bool drawRoundedRect = true;
    bool drawRoundedLines = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlRectangle rec = { ((float)rlGetScreenWidth() - width - 250)/2, (rlGetScreenHeight() - height)/2.0f, (float)width, (float)height };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawLine(560, 0, 560, rlGetScreenHeight(), rlFade(LIGHTGRAY, 0.6f));
            rlDrawRectangle(560, 0, rlGetScreenWidth() - 500, rlGetScreenHeight(), rlFade(LIGHTGRAY, 0.3f));

            if (drawRect) rlDrawRectangleRec(rec, rlFade(GOLD, 0.6f));
            if (drawRoundedRect) rlDrawRectangleRounded(rec, roundness, (int)segments, rlFade(MAROON, 0.2f));
            if (drawRoundedLines) rlDrawRectangleRoundedLinesEx(rec, roundness, (int)segments, lineThick, rlFade(MAROON, 0.4f));

            // Draw GUI controls
            //------------------------------------------------------------------------------
            GuiSliderBar((rlRectangle){ 640, 40, 105, 20 }, "Width", rlTextFormat("%.2f", width), &width, 0, (float)rlGetScreenWidth() - 300);
            GuiSliderBar((rlRectangle){ 640, 70, 105, 20 }, "Height", rlTextFormat("%.2f", height), &height, 0, (float)rlGetScreenHeight() - 50);
            GuiSliderBar((rlRectangle){ 640, 140, 105, 20 }, "Roundness", rlTextFormat("%.2f", roundness), &roundness, 0.0f, 1.0f);
            GuiSliderBar((rlRectangle){ 640, 170, 105, 20 }, "Thickness", rlTextFormat("%.2f", lineThick), &lineThick, 0, 20);
            GuiSliderBar((rlRectangle){ 640, 240, 105, 20}, "Segments", rlTextFormat("%.2f", segments), &segments, 0, 60);

            GuiCheckBox((rlRectangle){ 640, 320, 20, 20 }, "DrawRoundedRect", &drawRoundedRect);
            GuiCheckBox((rlRectangle){ 640, 350, 20, 20 }, "DrawRoundedLines", &drawRoundedLines);
            GuiCheckBox((rlRectangle){ 640, 380, 20, 20}, "DrawRect", &drawRect);
            //------------------------------------------------------------------------------

            rlDrawText(rlTextFormat("MODE: %s", (segments >= 4)? "MANUAL" : "AUTO"), 640, 280, 10, (segments >= 4)? MAROON : DARKGRAY);

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
