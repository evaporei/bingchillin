/*******************************************************************************************
*
*   raylib [models] example - Draw some basic geometric shapes (cube, sphere, cylinder...)
*
*   Example originally created with raylib 1.0, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - geometric shapes");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
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

            rlBeginMode3D(camera);

                rlDrawCube((rlVector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);
                rlDrawCubeWires((rlVector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, GOLD);
                rlDrawCubeWires((rlVector3){-4.0f, 0.0f, -2.0f}, 3.0f, 6.0f, 2.0f, MAROON);

                rlDrawSphere((rlVector3){-1.0f, 0.0f, -2.0f}, 1.0f, GREEN);
                rlDrawSphereWires((rlVector3){1.0f, 0.0f, 2.0f}, 2.0f, 16, 16, LIME);

                rlDrawCylinder((rlVector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, SKYBLUE);
                rlDrawCylinderWires((rlVector3){4.0f, 0.0f, -2.0f}, 1.0f, 2.0f, 3.0f, 4, DARKBLUE);
                rlDrawCylinderWires((rlVector3){4.5f, -1.0f, 2.0f}, 1.0f, 1.0f, 2.0f, 6, BROWN);

                rlDrawCylinder((rlVector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, GOLD);
                rlDrawCylinderWires((rlVector3){1.0f, 0.0f, -4.0f}, 0.0f, 1.5f, 3.0f, 8, PINK);

                rlDrawCapsule     ((rlVector3){-3.0f, 1.5f, -4.0f}, (rlVector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, VIOLET);
                rlDrawCapsuleWires((rlVector3){-3.0f, 1.5f, -4.0f}, (rlVector3){-4.0f, -1.0f, -4.0f}, 1.2f, 8, 8, PURPLE);

                rlDrawGrid(10, 1.0f);        // Draw a grid

            rlEndMode3D();

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