/*******************************************************************************************
*
*   raylib [core] example - Picking in 3d mode
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 3d picking");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlVector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    rlVector3 cubeSize = { 2.0f, 2.0f, 2.0f };

    rlRay ray = { 0 };                    // Picking line ray
    rlRayCollision collision = { 0 };     // rlRay collision hit info

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsCursorHidden()) rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Toggle camera controls
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (rlIsCursorHidden()) rlEnableCursor();
            else rlDisableCursor();
        }

        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!collision.hit)
            {
                ray = rlGetScreenToWorldRay(rlGetMousePosition(), camera);

                // Check collision between ray and box
                collision = rlGetRayCollisionBox(ray,
                            (rlBoundingBox){(rlVector3){ cubePosition.x - cubeSize.x/2, cubePosition.y - cubeSize.y/2, cubePosition.z - cubeSize.z/2 },
                                          (rlVector3){ cubePosition.x + cubeSize.x/2, cubePosition.y + cubeSize.y/2, cubePosition.z + cubeSize.z/2 }});
            }
            else collision.hit = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                if (collision.hit)
                {
                    rlDrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                    rlDrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

                    rlDrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
                }
                else
                {
                    rlDrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
                    rlDrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
                }

                rlDrawRay(ray, MAROON);
                rlDrawGrid(10, 1.0f);

            rlEndMode3D();

            rlDrawText("Try clicking on the box with your mouse!", 240, 10, 20, DARKGRAY);

            if (collision.hit) rlDrawText("BOX SELECTED", (screenWidth - rlMeasureText("BOX SELECTED", 30)) / 2, (int)(screenHeight * 0.1f), 30, GREEN);

            rlDrawText("Right click mouse to toggle camera controls", 10, 430, 10, GRAY);

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
