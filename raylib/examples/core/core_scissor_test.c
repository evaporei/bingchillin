/*******************************************************************************************
*
*   raylib [core] example - Scissor test
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.0
*
*   Example contributed by Chris Dill (@MysteriousSpace) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Chris Dill (@MysteriousSpace)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - scissor test");

    rlRectangle scissorArea = { 0, 0, 300, 300 };
    bool scissorMode = true;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_S)) scissorMode = !scissorMode;

        // Centre the scissor area around the mouse position
        scissorArea.x = rlGetMouseX() - scissorArea.width/2;
        scissorArea.y = rlGetMouseY() - scissorArea.height/2;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            if (scissorMode) rlBeginScissorMode((int)scissorArea.x, (int)scissorArea.y, (int)scissorArea.width, (int)scissorArea.height);

            // Draw full screen rectangle and some text
            // NOTE: Only part defined by scissor area will be rendered
            rlDrawRectangle(0, 0, rlGetScreenWidth(), rlGetScreenHeight(), RED);
            rlDrawText("Move the mouse around to reveal this text!", 190, 200, 20, LIGHTGRAY);

            if (scissorMode) rlEndScissorMode();

            rlDrawRectangleLinesEx(scissorArea, 1, BLACK);
            rlDrawText("Press S to toggle scissor test", 10, 10, 20, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
