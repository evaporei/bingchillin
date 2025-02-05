/*******************************************************************************************
*
*   raylib [models] example - loading gltf with animations
*
*   LIMITATIONS:
*     - Only supports 1 armature per file, and skips loading it if there are multiple armatures
*     - Only supports linear interpolation (default method in Blender when checked
*       "Always Sample Animations" when exporting a GLTF file)
*     - Only supports translation/rotation/scale animation channel.path,
*       weights not considered (i.e. morph targets)
*
*   Example originally created with raylib 3.7, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - loading gltf animations");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 6.0f, 6.0f, 6.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load gltf model
    rlModel model = rlLoadModel("resources/models/gltf/robot.glb");
    rlVector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position
    
    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    rlModelAnimation *modelAnimations = rlLoadModelAnimations("resources/models/gltf/robot.glb", &animsCount);

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        // Select current animation
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) animIndex = (animIndex + 1)%animsCount;
        else if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT)) animIndex = (animIndex + animsCount - 1)%animsCount;

        // Update model animation
        rlModelAnimation anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
        rlUpdateModelAnimation(model, anim, animCurrentFrame);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
                rlDrawModel(model, position, 1.0f, WHITE);    // Draw animated model
                rlDrawGrid(10, 1.0f);
            rlEndMode3D();

            rlDrawText("Use the LEFT/RIGHT mouse buttons to switch animation", 10, 10, 20, GRAY);
            rlDrawText(rlTextFormat("Animation: %s", anim.name), 10, rlGetScreenHeight() - 20, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(model);         // Unload model and meshes/material

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



