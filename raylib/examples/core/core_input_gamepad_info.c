/*******************************************************************************************
*
*   raylib [core] example - Gamepad information
*
*   NOTE: This example requires a Gamepad connected to the system
*         Check raylib.h for buttons configuration
*
*   Example originally created with raylib 4.6, last time updated with raylib 4.6
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
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - gamepad information");

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
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

            for (int i = 0, y = 5; i < 4; i++)     // MAX_GAMEPADS = 4
            {
                if (rlIsGamepadAvailable(i))
                {
                    rlDrawText(rlTextFormat("Gamepad name: %s", rlGetGamepadName(i)), 10, y, 10, BLACK);
                    y += 11;
                    rlDrawText(rlTextFormat("\tAxis count:   %d", rlGetGamepadAxisCount(i)), 10, y, 10, BLACK);
                    y += 11;

                    for (int axis = 0; axis < rlGetGamepadAxisCount(i); axis++)
                    {
                        rlDrawText(rlTextFormat("\tAxis %d = %f", axis, rlGetGamepadAxisMovement(i, axis)), 10, y, 10, BLACK);
                        y += 11;
                    }

                    for (int button = 0; button < 32; button++)
                    {
                        rlDrawText(rlTextFormat("\tButton %d = %d", button, rlIsGamepadButtonDown(i, button)), 10, y, 10, BLACK);
                        y += 11;
                    }
                }
            }

            rlDrawFPS(rlGetScreenWidth() - 100, 100);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
