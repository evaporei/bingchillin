/*******************************************************************************************
*
*   raylib [shapes] example - following eyes
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

#include <math.h>       // Required for: atan2f()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - following eyes");

    rlVector2 scleraLeftPosition = { rlGetScreenWidth()/2.0f - 100.0f, rlGetScreenHeight()/2.0f };
    rlVector2 scleraRightPosition = { rlGetScreenWidth()/2.0f + 100.0f, rlGetScreenHeight()/2.0f };
    float scleraRadius = 80;

    rlVector2 irisLeftPosition = { rlGetScreenWidth()/2.0f - 100.0f, rlGetScreenHeight()/2.0f };
    rlVector2 irisRightPosition = { rlGetScreenWidth()/2.0f + 100.0f, rlGetScreenHeight()/2.0f };
    float irisRadius = 24;

    float angle = 0.0f;
    float dx = 0.0f, dy = 0.0f, dxx = 0.0f, dyy = 0.0f;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        irisLeftPosition = rlGetMousePosition();
        irisRightPosition = rlGetMousePosition();

        // Check not inside the left eye sclera
        if (!rlCheckCollisionPointCircle(irisLeftPosition, scleraLeftPosition, scleraRadius - irisRadius))
        {
            dx = irisLeftPosition.x - scleraLeftPosition.x;
            dy = irisLeftPosition.y - scleraLeftPosition.y;

            angle = atan2f(dy, dx);

            dxx = (scleraRadius - irisRadius)*cosf(angle);
            dyy = (scleraRadius - irisRadius)*sinf(angle);

            irisLeftPosition.x = scleraLeftPosition.x + dxx;
            irisLeftPosition.y = scleraLeftPosition.y + dyy;
        }

        // Check not inside the right eye sclera
        if (!rlCheckCollisionPointCircle(irisRightPosition, scleraRightPosition, scleraRadius - irisRadius))
        {
            dx = irisRightPosition.x - scleraRightPosition.x;
            dy = irisRightPosition.y - scleraRightPosition.y;

            angle = atan2f(dy, dx);

            dxx = (scleraRadius - irisRadius)*cosf(angle);
            dyy = (scleraRadius - irisRadius)*sinf(angle);

            irisRightPosition.x = scleraRightPosition.x + dxx;
            irisRightPosition.y = scleraRightPosition.y + dyy;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawCircleV(scleraLeftPosition, scleraRadius, LIGHTGRAY);
            rlDrawCircleV(irisLeftPosition, irisRadius, BROWN);
            rlDrawCircleV(irisLeftPosition, 10, BLACK);

            rlDrawCircleV(scleraRightPosition, scleraRadius, LIGHTGRAY);
            rlDrawCircleV(irisRightPosition, irisRadius, DARKGREEN);
            rlDrawCircleV(irisRightPosition, 10, BLACK);

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