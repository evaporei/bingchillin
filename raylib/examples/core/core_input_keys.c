/*******************************************************************************************
*
*   raylib [core] example - Keyboard input
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
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

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    rlVector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (rlIsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (rlIsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        if (rlIsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            rlDrawCircleV(ballPosition, 50, MAROON);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}