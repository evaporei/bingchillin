/*******************************************************************************************
*
*   raylib [shapes] example - Colors palette
*
*   Example originally created with raylib 1.0, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_COLORS_COUNT    21          // Number of colors available

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - colors palette");

    rlColor colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };

    const char *colorNames[MAX_COLORS_COUNT] = {
        "DARKGRAY", "MAROON", "ORANGE", "DARKGREEN", "DARKBLUE", "DARKPURPLE",
        "DARKBROWN", "GRAY", "RED", "GOLD", "LIME", "BLUE", "VIOLET", "BROWN",
        "LIGHTGRAY", "PINK", "YELLOW", "GREEN", "SKYBLUE", "PURPLE", "BEIGE" };

    rlRectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };     // Rectangles array

    // Fills colorsRecs data (for every rectangle)
    for (int i = 0; i < MAX_COLORS_COUNT; i++)
    {
        colorsRecs[i].x = 20.0f + 100.0f *(i%7) + 10.0f *(i%7);
        colorsRecs[i].y = 80.0f + 100.0f *(i/7) + 10.0f *(i/7);
        colorsRecs[i].width = 100.0f;
        colorsRecs[i].height = 100.0f;
    }

    int colorState[MAX_COLORS_COUNT] = { 0 };           // rlColor state: 0-DEFAULT, 1-MOUSE_HOVER

    rlVector2 mousePoint = { 0.0f, 0.0f };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = rlGetMousePosition();

        for (int i = 0; i < MAX_COLORS_COUNT; i++)
        {
            if (rlCheckCollisionPointRec(mousePoint, colorsRecs[i])) colorState[i] = 1;
            else colorState[i] = 0;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("raylib colors palette", 28, 42, 20, BLACK);
            rlDrawText("press SPACE to see all colors", rlGetScreenWidth() - 180, rlGetScreenHeight() - 40, 10, GRAY);

            for (int i = 0; i < MAX_COLORS_COUNT; i++)    // Draw all rectangles
            {
                rlDrawRectangleRec(colorsRecs[i], rlFade(colors[i], colorState[i]? 0.6f : 1.0f));

                if (rlIsKeyDown(KEY_SPACE) || colorState[i])
                {
                    rlDrawRectangle((int)colorsRecs[i].x, (int)(colorsRecs[i].y + colorsRecs[i].height - 26), (int)colorsRecs[i].width, 20, BLACK);
                    rlDrawRectangleLinesEx(colorsRecs[i], 6, rlFade(BLACK, 0.3f));
                    rlDrawText(colorNames[i], (int)(colorsRecs[i].x + colorsRecs[i].width - rlMeasureText(colorNames[i], 10) - 12),
                        (int)(colorsRecs[i].y + colorsRecs[i].height - 20), 10, colors[i]);
                }
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}