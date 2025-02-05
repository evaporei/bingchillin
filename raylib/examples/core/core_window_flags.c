/*******************************************************************************************
*
*   raylib [core] example - window flags
*
*   Example originally created with raylib 3.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 Ramon Santamaria (@raysan5)
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

    // Possible window flags
    /*
    FLAG_VSYNC_HINT
    FLAG_FULLSCREEN_MODE    -> not working properly -> wrong scaling!
    FLAG_WINDOW_RESIZABLE
    FLAG_WINDOW_UNDECORATED
    FLAG_WINDOW_TRANSPARENT
    FLAG_WINDOW_HIDDEN
    FLAG_WINDOW_MINIMIZED   -> Not supported on window creation
    FLAG_WINDOW_MAXIMIZED   -> Not supported on window creation
    FLAG_WINDOW_UNFOCUSED
    FLAG_WINDOW_TOPMOST
    FLAG_WINDOW_HIGHDPI     -> errors after minimize-resize, fb size is recalculated
    FLAG_WINDOW_ALWAYS_RUN
    FLAG_MSAA_4X_HINT
    */

    // Set configuration flags for window creation
    //rlSetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - window flags");

    rlVector2 ballPosition = { rlGetScreenWidth() / 2.0f, rlGetScreenHeight() / 2.0f };
    rlVector2 ballSpeed = { 5.0f, 4.0f };
    float ballRadius = 20;

    int framesCounter = 0;

    //rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (rlIsKeyPressed(KEY_F)) rlToggleFullscreen();  // modifies window size when scaling!

        if (rlIsKeyPressed(KEY_R))
        {
            if (rlIsWindowState(FLAG_WINDOW_RESIZABLE)) rlClearWindowState(FLAG_WINDOW_RESIZABLE);
            else rlSetWindowState(FLAG_WINDOW_RESIZABLE);
        }

        if (rlIsKeyPressed(KEY_D))
        {
            if (rlIsWindowState(FLAG_WINDOW_UNDECORATED)) rlClearWindowState(FLAG_WINDOW_UNDECORATED);
            else rlSetWindowState(FLAG_WINDOW_UNDECORATED);
        }

        if (rlIsKeyPressed(KEY_H))
        {
            if (!rlIsWindowState(FLAG_WINDOW_HIDDEN)) rlSetWindowState(FLAG_WINDOW_HIDDEN);

            framesCounter = 0;
        }

        if (rlIsWindowState(FLAG_WINDOW_HIDDEN))
        {
            framesCounter++;
            if (framesCounter >= 240) rlClearWindowState(FLAG_WINDOW_HIDDEN); // Show window after 3 seconds
        }

        if (rlIsKeyPressed(KEY_N))
        {
            if (!rlIsWindowState(FLAG_WINDOW_MINIMIZED)) rlMinimizeWindow();

            framesCounter = 0;
        }

        if (rlIsWindowState(FLAG_WINDOW_MINIMIZED))
        {
            framesCounter++;
            if (framesCounter >= 240) rlRestoreWindow(); // Restore window after 3 seconds
        }

        if (rlIsKeyPressed(KEY_M))
        {
            // NOTE: Requires FLAG_WINDOW_RESIZABLE enabled!
            if (rlIsWindowState(FLAG_WINDOW_MAXIMIZED)) rlRestoreWindow();
            else rlMaximizeWindow();
        }

        if (rlIsKeyPressed(KEY_U))
        {
            if (rlIsWindowState(FLAG_WINDOW_UNFOCUSED)) rlClearWindowState(FLAG_WINDOW_UNFOCUSED);
            else rlSetWindowState(FLAG_WINDOW_UNFOCUSED);
        }

        if (rlIsKeyPressed(KEY_T))
        {
            if (rlIsWindowState(FLAG_WINDOW_TOPMOST)) rlClearWindowState(FLAG_WINDOW_TOPMOST);
            else rlSetWindowState(FLAG_WINDOW_TOPMOST);
        }

        if (rlIsKeyPressed(KEY_A))
        {
            if (rlIsWindowState(FLAG_WINDOW_ALWAYS_RUN)) rlClearWindowState(FLAG_WINDOW_ALWAYS_RUN);
            else rlSetWindowState(FLAG_WINDOW_ALWAYS_RUN);
        }

        if (rlIsKeyPressed(KEY_V))
        {
            if (rlIsWindowState(FLAG_VSYNC_HINT)) rlClearWindowState(FLAG_VSYNC_HINT);
            else rlSetWindowState(FLAG_VSYNC_HINT);
        }

        // Bouncing ball logic
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;
        if ((ballPosition.x >= (rlGetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
        if ((ballPosition.y >= (rlGetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        rlBeginDrawing();

        if (rlIsWindowState(FLAG_WINDOW_TRANSPARENT)) rlClearBackground(BLANK);
        else rlClearBackground(RAYWHITE);

        rlDrawCircleV(ballPosition, ballRadius, MAROON);
        rlDrawRectangleLinesEx((rlRectangle) { 0, 0, (float)rlGetScreenWidth(), (float)rlGetScreenHeight() }, 4, RAYWHITE);

        rlDrawCircleV(rlGetMousePosition(), 10, DARKBLUE);

        rlDrawFPS(10, 10);

        rlDrawText(rlTextFormat("Screen Size: [%i, %i]", rlGetScreenWidth(), rlGetScreenHeight()), 10, 40, 10, GREEN);

        // Draw window state info
        rlDrawText("Following flags can be set after window creation:", 10, 60, 10, GRAY);
        if (rlIsWindowState(FLAG_FULLSCREEN_MODE)) rlDrawText("[F] FLAG_FULLSCREEN_MODE: on", 10, 80, 10, LIME);
        else rlDrawText("[F] FLAG_FULLSCREEN_MODE: off", 10, 80, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_RESIZABLE)) rlDrawText("[R] FLAG_WINDOW_RESIZABLE: on", 10, 100, 10, LIME);
        else rlDrawText("[R] FLAG_WINDOW_RESIZABLE: off", 10, 100, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_UNDECORATED)) rlDrawText("[D] FLAG_WINDOW_UNDECORATED: on", 10, 120, 10, LIME);
        else rlDrawText("[D] FLAG_WINDOW_UNDECORATED: off", 10, 120, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_HIDDEN)) rlDrawText("[H] FLAG_WINDOW_HIDDEN: on", 10, 140, 10, LIME);
        else rlDrawText("[H] FLAG_WINDOW_HIDDEN: off", 10, 140, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_MINIMIZED)) rlDrawText("[N] FLAG_WINDOW_MINIMIZED: on", 10, 160, 10, LIME);
        else rlDrawText("[N] FLAG_WINDOW_MINIMIZED: off", 10, 160, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_MAXIMIZED)) rlDrawText("[M] FLAG_WINDOW_MAXIMIZED: on", 10, 180, 10, LIME);
        else rlDrawText("[M] FLAG_WINDOW_MAXIMIZED: off", 10, 180, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_UNFOCUSED)) rlDrawText("[G] FLAG_WINDOW_UNFOCUSED: on", 10, 200, 10, LIME);
        else rlDrawText("[U] FLAG_WINDOW_UNFOCUSED: off", 10, 200, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_TOPMOST)) rlDrawText("[T] FLAG_WINDOW_TOPMOST: on", 10, 220, 10, LIME);
        else rlDrawText("[T] FLAG_WINDOW_TOPMOST: off", 10, 220, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_ALWAYS_RUN)) rlDrawText("[A] FLAG_WINDOW_ALWAYS_RUN: on", 10, 240, 10, LIME);
        else rlDrawText("[A] FLAG_WINDOW_ALWAYS_RUN: off", 10, 240, 10, MAROON);
        if (rlIsWindowState(FLAG_VSYNC_HINT)) rlDrawText("[V] FLAG_VSYNC_HINT: on", 10, 260, 10, LIME);
        else rlDrawText("[V] FLAG_VSYNC_HINT: off", 10, 260, 10, MAROON);

        rlDrawText("Following flags can only be set before window creation:", 10, 300, 10, GRAY);
        if (rlIsWindowState(FLAG_WINDOW_HIGHDPI)) rlDrawText("FLAG_WINDOW_HIGHDPI: on", 10, 320, 10, LIME);
        else rlDrawText("FLAG_WINDOW_HIGHDPI: off", 10, 320, 10, MAROON);
        if (rlIsWindowState(FLAG_WINDOW_TRANSPARENT)) rlDrawText("FLAG_WINDOW_TRANSPARENT: on", 10, 340, 10, LIME);
        else rlDrawText("FLAG_WINDOW_TRANSPARENT: off", 10, 340, 10, MAROON);
        if (rlIsWindowState(FLAG_MSAA_4X_HINT)) rlDrawText("FLAG_MSAA_4X_HINT: on", 10, 360, 10, LIME);
        else rlDrawText("FLAG_MSAA_4X_HINT: off", 10, 360, 10, MAROON);

        rlEndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
