/*******************************************************************************************
*
*   raylib [text] example - Text formatting
*
*   Example originally created with raylib 1.1, last time updated with raylib 3.0
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

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - text formatting");

    int score = 100020;
    int hiscore = 200450;
    int lives = 5;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText(rlTextFormat("Score: %08i", score), 200, 80, 20, RED);

            rlDrawText(rlTextFormat("HiScore: %08i", hiscore), 200, 120, 20, GREEN);

            rlDrawText(rlTextFormat("Lives: %02i", lives), 200, 160, 40, BLUE);

            rlDrawText(rlTextFormat("Elapsed Time: %02.02f ms", rlGetFrameTime()*1000), 200, 220, 20, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}