/*******************************************************************************************
*
*   raylib [core] example - Mouse input
*
*   Example originally created with raylib 1.0, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - mouse input");

    rlVector2 ballPosition = { -100.0f, -100.0f };
    rlColor ballColor = DARKBLUE;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        ballPosition = rlGetMousePosition();

        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawCircleV(ballPosition, 40, ballColor);

            rlDrawText("move ball with mouse and click mouse button to change color", 10, 10, 20, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}