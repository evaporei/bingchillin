/*******************************************************************************************
*
*   raylib [models] example - Cubicmap loading and drawing
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - cubesmap loading and drawing");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 16.0f, 14.0f, 16.0f };     // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    rlImage image = rlLoadImage("resources/cubicmap.png");      // Load cubicmap image (RAM)
    Texture2D cubicmap = rlLoadTextureFromImage(image);       // Convert image to texture to display (VRAM)

    rlMesh mesh = rlGenMeshCubicmap(image, (rlVector3){ 1.0f, 1.0f, 1.0f });
    rlModel model = rlLoadModelFromMesh(mesh);

    // NOTE: By default each cube is mapped to one part of texture atlas
    Texture2D texture = LoadTexture("resources/cubicmap_atlas.png");    // Load map texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;    // Set map diffuse texture

    rlVector3 mapPosition = { -16.0f, 0.0f, -8.0f };          // Set model position

    rlUnloadImage(image);     // Unload cubesmap image from RAM, already uploaded to VRAM

    bool pause = false;     // Pause camera orbital rotation (and zoom)

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_P)) pause = !pause;

        if (!pause) rlUpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawModel(model, mapPosition, 1.0f, WHITE);

            rlEndMode3D();

            rlDrawTextureEx(cubicmap, (rlVector2){ screenWidth - cubicmap.width*4.0f - 20, 20.0f }, 0.0f, 4.0f, WHITE);
            rlDrawRectangleLines(screenWidth - cubicmap.width*4 - 20, 20, cubicmap.width*4, cubicmap.height*4, GREEN);

            rlDrawText("cubicmap image used to", 658, 90, 10, GRAY);
            rlDrawText("generate map 3d model", 658, 104, 10, GRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(cubicmap);    // Unload cubicmap texture
    UnloadTexture(texture);     // Unload map texture
    rlUnloadModel(model);         // Unload map model

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
