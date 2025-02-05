/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera free
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
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

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    rlCamera3D camera = { 0 };
    camera.position = (rlVector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlVector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FREE);

        if (rlIsKeyPressed('Z')) camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                rlDrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                rlDrawGrid(10, 1.0f);

            rlEndMode3D();

            rlDrawRectangle( 10, 10, 320, 93, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines( 10, 10, 320, 93, BLUE);

            rlDrawText("Free camera default controls:", 20, 20, 10, BLACK);
            rlDrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
            rlDrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
            rlDrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
