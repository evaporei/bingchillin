/*******************************************************************************************
*
*   raylib [models] example - Show the difference between perspective and orthographic projection
*
*   Example originally created with raylib 2.0, last time updated with raylib 3.7
*
*   Example contributed by Max Danielsson (@autious) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2018-2024 Max Danielsson (@autious) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define FOVY_PERSPECTIVE    45.0f
#define WIDTH_ORTHOGRAPHIC  10.0f

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
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, FOVY_PERSPECTIVE, CAMERA_PERSPECTIVE };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                camera.fovy = WIDTH_ORTHOGRAPHIC;
                camera.projection = CAMERA_ORTHOGRAPHIC;
            }
            else
            {
                camera.fovy = FOVY_PERSPECTIVE;
                camera.projection = CAMERA_PERSPECTIVE;
            }
        }
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

                rlDrawGrid(10, 1.0f);        // Draw a grid

            rlEndMode3D();

            rlDrawText("Press Spacebar to switch camera type", 10, rlGetScreenHeight() - 30, 20, DARKGRAY);

            if (camera.projection == CAMERA_ORTHOGRAPHIC) rlDrawText("ORTHOGRAPHIC", 10, 40, 20, BLACK);
            else if (camera.projection == CAMERA_PERSPECTIVE) rlDrawText("PERSPECTIVE", 10, 40, 20, BLACK);

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
