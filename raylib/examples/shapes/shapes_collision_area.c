/*******************************************************************************************
*
*   raylib [shapes] example - collision area
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

#include <stdlib.h>     // Required for: abs()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - collision area");

    // Box A: Moving box
    rlRectangle boxA = { 10, rlGetScreenHeight()/2.0f - 50, 200, 100 };
    int boxASpeedX = 4;

    // Box B: Mouse moved box
    rlRectangle boxB = { rlGetScreenWidth()/2.0f - 30, rlGetScreenHeight()/2.0f - 30, 60, 60 };

    rlRectangle boxCollision = { 0 }; // Collision rectangle

    int screenUpperLimit = 40;      // Top menu limits

    bool pause = false;             // Movement pause
    bool collision = false;         // Collision detection

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        // Move box if not paused
        if (!pause) boxA.x += boxASpeedX;

        // Bounce box on x screen limits
        if (((boxA.x + boxA.width) >= rlGetScreenWidth()) || (boxA.x <= 0)) boxASpeedX *= -1;

        // Update player-controlled-box (box02)
        boxB.x = rlGetMouseX() - boxB.width/2;
        boxB.y = rlGetMouseY() - boxB.height/2;

        // Make sure Box B does not go out of move area limits
        if ((boxB.x + boxB.width) >= rlGetScreenWidth()) boxB.x = rlGetScreenWidth() - boxB.width;
        else if (boxB.x <= 0) boxB.x = 0;

        if ((boxB.y + boxB.height) >= rlGetScreenHeight()) boxB.y = rlGetScreenHeight() - boxB.height;
        else if (boxB.y <= screenUpperLimit) boxB.y = (float)screenUpperLimit;

        // Check boxes collision
        collision = rlCheckCollisionRecs(boxA, boxB);

        // Get collision rectangle (only on collision)
        if (collision) boxCollision = rlGetCollisionRec(boxA, boxB);

        // Pause Box A movement
        if (rlIsKeyPressed(KEY_SPACE)) pause = !pause;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawRectangle(0, 0, screenWidth, screenUpperLimit, collision? RED : BLACK);

            rlDrawRectangleRec(boxA, GOLD);
            rlDrawRectangleRec(boxB, BLUE);

            if (collision)
            {
                // Draw collision area
                rlDrawRectangleRec(boxCollision, LIME);

                // Draw collision message
                rlDrawText("COLLISION!", rlGetScreenWidth()/2 - rlMeasureText("COLLISION!", 20)/2, screenUpperLimit/2 - 10, 20, BLACK);

                // Draw collision area
                rlDrawText(rlTextFormat("Collision Area: %i", (int)boxCollision.width*(int)boxCollision.height), rlGetScreenWidth()/2 - 100, screenUpperLimit + 10, 20, BLACK);
            }

            // Draw help instructions
            rlDrawText("Press SPACE to PAUSE/RESUME", 20, screenHeight - 35, 20, LIGHTGRAY);

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
