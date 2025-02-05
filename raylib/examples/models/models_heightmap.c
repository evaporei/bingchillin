/*******************************************************************************************
*
*   raylib [models] example - Heightmap loading and drawing
*
*   Example originally created with raylib 1.8, last time updated with raylib 3.5
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - heightmap loading and drawing");

    // Define our custom camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 18.0f, 21.0f, 18.0f };     // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    rlImage image = rlLoadImage("resources/heightmap.png");     // Load heightmap image (RAM)
    Texture2D texture = rlLoadTextureFromImage(image);        // Convert image to texture (VRAM)

    rlMesh mesh = rlGenMeshHeightmap(image, (rlVector3){ 16, 8, 16 }); // Generate heightmap mesh (RAM and VRAM)
    rlModel model = rlLoadModelFromMesh(mesh);                  // Load model from generated mesh

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture
    rlVector3 mapPosition = { -8.0f, 0.0f, -8.0f };           // Define model position

    rlUnloadImage(image);             // Unload heightmap image from RAM, already uploaded to VRAM

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawModel(model, mapPosition, 1.0f, RED);

                rlDrawGrid(20, 1.0f);

            rlEndMode3D();

            rlDrawTexture(texture, screenWidth - texture.width - 20, 20, WHITE);
            rlDrawRectangleLines(screenWidth - texture.width - 20, 20, texture.width, texture.height, GREEN);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);     // Unload texture
    rlUnloadModel(model);         // Unload model

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}