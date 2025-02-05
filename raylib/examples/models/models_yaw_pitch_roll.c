/*******************************************************************************************
*
*   raylib [models] example - Plane rotations (yaw, pitch, roll)
*
*   Example originally created with raylib 1.8, last time updated with raylib 4.0
*
*   Example contributed by Berni (@Berni8k) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Berni (@Berni8k) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"        // Required for: MatrixRotateXYZ()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    //rlSetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - plane rotations (yaw, pitch, roll)");

    Camera camera = { 0 };
    camera.position = (rlVector3){ 0.0f, 50.0f, -120.0f };// Camera position perspective
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 30.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera type

    rlModel model = rlLoadModel("resources/models/obj/plane.obj");                  // Load model
    Texture2D texture = LoadTexture("resources/models/obj/plane_diffuse.png");  // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Plane pitch (x-axis) controls
        if (rlIsKeyDown(KEY_DOWN)) pitch += 0.6f;
        else if (rlIsKeyDown(KEY_UP)) pitch -= 0.6f;
        else
        {
            if (pitch > 0.3f) pitch -= 0.3f;
            else if (pitch < -0.3f) pitch += 0.3f;
        }

        // Plane yaw (y-axis) controls
        if (rlIsKeyDown(KEY_S)) yaw -= 1.0f;
        else if (rlIsKeyDown(KEY_A)) yaw += 1.0f;
        else
        {
            if (yaw > 0.0f) yaw -= 0.5f;
            else if (yaw < 0.0f) yaw += 0.5f;
        }

        // Plane roll (z-axis) controls
        if (rlIsKeyDown(KEY_LEFT)) roll -= 1.0f;
        else if (rlIsKeyDown(KEY_RIGHT)) roll += 1.0f;
        else
        {
            if (roll > 0.0f) roll -= 0.5f;
            else if (roll < 0.0f) roll += 0.5f;
        }

        // Tranformation matrix for rotations
        model.transform = MatrixRotateXYZ((rlVector3){ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            // Draw 3D model (recomended to draw 3D always before 2D)
            rlBeginMode3D(camera);

                rlDrawModel(model, (rlVector3){ 0.0f, -8.0f, 0.0f }, 1.0f, WHITE);   // Draw 3d model with texture
                rlDrawGrid(10, 10.0f);

            rlEndMode3D();

            // Draw controls info
            rlDrawRectangle(30, 370, 260, 70, rlFade(GREEN, 0.5f));
            rlDrawRectangleLines(30, 370, 260, 70, rlFade(DARKGREEN, 0.5f));
            rlDrawText("Pitch controlled with: KEY_UP / KEY_DOWN", 40, 380, 10, DARKGRAY);
            rlDrawText("Roll controlled with: KEY_LEFT / KEY_RIGHT", 40, 400, 10, DARKGRAY);
            rlDrawText("Yaw controlled with: KEY_A / KEY_S", 40, 420, 10, DARKGRAY);

            rlDrawText("(c) WWI Plane rlModel created by GiaHanLam", screenWidth - 240, screenHeight - 20, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(model);     // Unload model data
    UnloadTexture(texture); // Unload texture data

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
