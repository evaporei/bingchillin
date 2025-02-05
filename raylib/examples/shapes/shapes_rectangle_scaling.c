/*******************************************************************************************
*
*   raylib [shapes] example - rectangle scaling by mouse
*
*   Example originally created with raylib 2.5, last time updated with raylib 2.5
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2018-2024 Vlad Adrian (@demizdor) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MOUSE_SCALE_MARK_SIZE   12

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - rectangle scaling mouse");

    rlRectangle rec = { 100, 100, 200, 80 };

    rlVector2 mousePosition = { 0 };

    bool mouseScaleReady = false;
    bool mouseScaleMode = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePosition = rlGetMousePosition();

        if (rlCheckCollisionPointRec(mousePosition, (rlRectangle){ rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE }))
        {
            mouseScaleReady = true;
            if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT)) mouseScaleMode = true;
        }
        else mouseScaleReady = false;

        if (mouseScaleMode)
        {
            mouseScaleReady = true;

            rec.width = (mousePosition.x - rec.x);
            rec.height = (mousePosition.y - rec.y);

            // Check minimum rec size
            if (rec.width < MOUSE_SCALE_MARK_SIZE) rec.width = MOUSE_SCALE_MARK_SIZE;
            if (rec.height < MOUSE_SCALE_MARK_SIZE) rec.height = MOUSE_SCALE_MARK_SIZE;
            
            // Check maximum rec size
            if (rec.width > (rlGetScreenWidth() - rec.x)) rec.width = rlGetScreenWidth() - rec.x;
            if (rec.height > (rlGetScreenHeight() - rec.y)) rec.height = rlGetScreenHeight() - rec.y;

            if (rlIsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseScaleMode = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("Scale rectangle dragging from bottom-right corner!", 10, 10, 20, GRAY);

            rlDrawRectangleRec(rec, rlFade(GREEN, 0.5f));

            if (mouseScaleReady)
            {
                rlDrawRectangleLinesEx(rec, 1, RED);
                rlDrawTriangle((rlVector2){ rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height },
                             (rlVector2){ rec.x + rec.width, rec.y + rec.height },
                             (rlVector2){ rec.x + rec.width, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE }, RED);
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}