/*******************************************************************************************
*
*   raylib [shapes] example - bouncing ball
*
*   Example originally created with raylib 2.5, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);
    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    rlVector2 ballPosition = { rlGetScreenWidth()/2.0f, rlGetScreenHeight()/2.0f };
    rlVector2 ballSpeed = { 5.0f, 4.0f };
    int ballRadius = 20;

    bool pause = 0;
    int framesCounter = 0;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        {
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            // Check walls collision for bouncing
            if ((ballPosition.x >= (rlGetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
            if ((ballPosition.y >= (rlGetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;
        }
        else framesCounter++;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawCircleV(ballPosition, (float)ballRadius, MAROON);
            rlDrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, rlGetScreenHeight() - 25, 20, LIGHTGRAY);

            // On pause, we draw a blinking message
            if (pause && ((framesCounter/30)%2)) rlDrawText("PAUSED", 350, 200, 30, GRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}