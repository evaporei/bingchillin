/*******************************************************************************************
*
*   raylib [models] example - Detect basic 3d collisions (box vs sphere vs box)
*
*   Example originally created with raylib 1.3, last time updated with raylib 3.5
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");

    // Define the camera to look into our 3d world
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    rlVector3 playerPosition = { 0.0f, 1.0f, 2.0f };
    rlVector3 playerSize = { 1.0f, 2.0f, 1.0f };
    rlColor playerColor = GREEN;

    rlVector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
    rlVector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };

    rlVector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
    float enemySphereSize = 1.5f;

    bool collision = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Move player
        if (rlIsKeyDown(KEY_RIGHT)) playerPosition.x += 0.2f;
        else if (rlIsKeyDown(KEY_LEFT)) playerPosition.x -= 0.2f;
        else if (rlIsKeyDown(KEY_DOWN)) playerPosition.z += 0.2f;
        else if (rlIsKeyDown(KEY_UP)) playerPosition.z -= 0.2f;

        collision = false;

        // Check collisions player vs enemy-box
        if (rlCheckCollisionBoxes(
            (rlBoundingBox){(rlVector3){ playerPosition.x - playerSize.x/2,
                                     playerPosition.y - playerSize.y/2,
                                     playerPosition.z - playerSize.z/2 },
                          (rlVector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2,
                                     playerPosition.z + playerSize.z/2 }},
            (rlBoundingBox){(rlVector3){ enemyBoxPos.x - enemyBoxSize.x/2,
                                     enemyBoxPos.y - enemyBoxSize.y/2,
                                     enemyBoxPos.z - enemyBoxSize.z/2 },
                          (rlVector3){ enemyBoxPos.x + enemyBoxSize.x/2,
                                     enemyBoxPos.y + enemyBoxSize.y/2,
                                     enemyBoxPos.z + enemyBoxSize.z/2 }})) collision = true;

        // Check collisions player vs enemy-sphere
        if (rlCheckCollisionBoxSphere(
            (rlBoundingBox){(rlVector3){ playerPosition.x - playerSize.x/2,
                                     playerPosition.y - playerSize.y/2,
                                     playerPosition.z - playerSize.z/2 },
                          (rlVector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2,
                                     playerPosition.z + playerSize.z/2 }},
            enemySpherePos, enemySphereSize)) collision = true;

        if (collision) playerColor = RED;
        else playerColor = GREEN;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                // Draw enemy-box
                rlDrawCube(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
                rlDrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

                // Draw enemy-sphere
                rlDrawSphere(enemySpherePos, enemySphereSize, GRAY);
                rlDrawSphereWires(enemySpherePos, enemySphereSize, 16, 16, DARKGRAY);

                // Draw player
                rlDrawCubeV(playerPosition, playerSize, playerColor);

                rlDrawGrid(10, 1.0f);        // Draw a grid

            rlEndMode3D();

            rlDrawText("Move player with arrow keys to collide", 220, 40, 20, GRAY);

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