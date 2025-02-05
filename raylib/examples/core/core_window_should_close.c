/*******************************************************************************************
*
*   raylib [core] example - Window should close
*
*   Example originally created with raylib 4.2, last time updated with raylib 4.2
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
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - window should close");
    
    rlSetExitKey(KEY_NULL);       // Disable KEY_ESCAPE to close window, X-button still works
    
    bool exitWindowRequested = false;   // Flag to request window to exit
    bool exitWindow = false;    // Flag to set window to exit

    rlSetTargetFPS(60);           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!exitWindow)
    {
        // Update
        //----------------------------------------------------------------------------------
        // Detect if X-button or KEY_ESCAPE have been pressed to close window
        if (rlWindowShouldClose() || rlIsKeyPressed(KEY_ESCAPE)) exitWindowRequested = true;
        
        if (exitWindowRequested)
        {
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            
            if (rlIsKeyPressed(KEY_Y)) exitWindow = true;
            else if (rlIsKeyPressed(KEY_N)) exitWindowRequested = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            if (exitWindowRequested)
            {
                rlDrawRectangle(0, 100, screenWidth, 200, BLACK);
                rlDrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
            }
            else rlDrawText("Try to close the window to get confirmation message!", 120, 200, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
