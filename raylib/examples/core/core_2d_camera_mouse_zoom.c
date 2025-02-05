/*******************************************************************************************
*
*   raylib [core] example - 2d camera mouse zoom
*
*   Example originally created with raylib 4.2, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2024 Jeffery Myers (@JeffM2501)
*
********************************************************************************************/

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main ()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera mouse zoom");

    rlCamera2D camera = { 0 };
    camera.zoom = 1.0f;

    int zoomMode = 0;   // 0-Mouse Wheel, 1-Mouse Move

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_ONE)) zoomMode = 0;
        else if (rlIsKeyPressed(KEY_TWO)) zoomMode = 1;
        
        // Translate based on mouse right click
        if (rlIsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            rlVector2 delta = rlGetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (zoomMode == 0)
        {
            // Zoom based on mouse wheel
            float wheel = rlGetMouseWheelMove();
            if (wheel != 0)
            {
                // Get the world point that is under the mouse
                rlVector2 mouseWorldPos = rlGetScreenToWorld2D(rlGetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = rlGetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;

                // Zoom increment
                float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
                if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        else
        {
            // Zoom based on mouse left click
            if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // Get the world point that is under the mouse
                rlVector2 mouseWorldPos = rlGetScreenToWorld2D(rlGetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = rlGetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;
            }
            if (rlIsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                // Zoom increment
                float deltaX = rlGetMouseDelta().x;
                float scaleFactor = 1.0f + (0.01f*fabsf(deltaX));
                if (deltaX < 0) scaleFactor = 1.0f/scaleFactor;
                camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();
            rlClearBackground(RAYWHITE);

            rlBeginMode2D(camera);

                // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
                // just so we have something in the XY plane
                rlPushMatrix();
                    rlTranslatef(0, 25*50, 0);
                    rlRotatef(90, 1, 0, 0);
                    rlDrawGrid(100, 50);
                rlPopMatrix();

                // Draw a reference circle
                rlDrawCircle(rlGetScreenWidth()/2, rlGetScreenHeight()/2, 50, MAROON);
                
            rlEndMode2D();

            rlDrawText("[1][2] Select mouse zoom mode (Wheel or Move)", 20, 20, 20, DARKGRAY);
            if (zoomMode == 0) rlDrawText("Mouse right button drag to move, mouse wheel to zoom", 20, 50, 20, DARKGRAY);
            else rlDrawText("Mouse right button drag to move, mouse press and move to zoom", 20, 50, 20, DARKGRAY);
        
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}