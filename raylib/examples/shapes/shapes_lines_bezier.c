/*******************************************************************************************
*
*   raylib [shapes] example - Cubic-bezier lines
*
*   Example originally created with raylib 1.7, last time updated with raylib 1.7
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Ramon Santamaria (@raysan5)
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

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);
    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - cubic-bezier lines");

    rlVector2 startPoint = { 30, 30 };
    rlVector2 endPoint = { (float)screenWidth - 30, (float)screenHeight - 30 };
    bool moveStartPoint = false;
    bool moveEndPoint = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlVector2 mouse = rlGetMousePosition();

        if (rlCheckCollisionPointCircle(mouse, startPoint, 10.0f) && rlIsMouseButtonDown(MOUSE_BUTTON_LEFT)) moveStartPoint = true;
        else if (rlCheckCollisionPointCircle(mouse, endPoint, 10.0f) && rlIsMouseButtonDown(MOUSE_BUTTON_LEFT)) moveEndPoint = true;

        if (moveStartPoint)
        {
            startPoint = mouse;
            if (rlIsMouseButtonReleased(MOUSE_BUTTON_LEFT)) moveStartPoint = false;
        }

        if (moveEndPoint)
        {
            endPoint = mouse;
            if (rlIsMouseButtonReleased(MOUSE_BUTTON_LEFT)) moveEndPoint = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("MOVE START-END POINTS WITH MOUSE", 15, 20, 20, GRAY);

            // Draw line Cubic Bezier, in-out interpolation (easing), no control points
            rlDrawLineBezier(startPoint, endPoint, 4.0f, BLUE);
            
            // Draw start-end spline circles with some details
            rlDrawCircleV(startPoint, rlCheckCollisionPointCircle(mouse, startPoint, 10.0f)? 14.0f : 8.0f, moveStartPoint? RED : BLUE);
            rlDrawCircleV(endPoint, rlCheckCollisionPointCircle(mouse, endPoint, 10.0f)? 14.0f : 8.0f, moveEndPoint? RED : BLUE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
