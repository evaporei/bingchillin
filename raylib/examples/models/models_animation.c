/*******************************************************************************************
*
*   raylib [models] example - Load 3d model with animations and play them
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.5
*
*   Example contributed by Culacant (@culacant) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Culacant (@culacant) and Ramon Santamaria (@raysan5)
*
********************************************************************************************
*
*   NOTE: To export a model from blender, make sure it is not posed, the vertices need to be 
*         in the same position as they would be in edit mode and the scale of your models is 
*         set to 0. Scaling can be done from the export menu.
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

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - model animation");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    rlModel model = rlLoadModel("resources/models/iqm/guy.iqm");                    // Load the animated model mesh and basic data
    Texture2D texture = LoadTexture("resources/models/iqm/guytex.png");         // Load model texture and set material
    rlSetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);     // Set model material map texture

    rlVector3 position = { 0.0f, 0.0f, 0.0f };            // Set model position

    // Load animation data
    int animsCount = 0;
    rlModelAnimation *anims = rlLoadModelAnimations("resources/models/iqm/guyanim.iqm", &animsCount);
    int animFrameCounter = 0;

    rlDisableCursor();                    // Catch cursor
    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Play animation when spacebar is held down
        if (rlIsKeyDown(KEY_SPACE))
        {
            animFrameCounter++;
            rlUpdateModelAnimation(model, anims[0], animFrameCounter);
            if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawModelEx(model, position, (rlVector3){ 1.0f, 0.0f, 0.0f }, -90.0f, (rlVector3){ 1.0f, 1.0f, 1.0f }, WHITE);

                for (int i = 0; i < model.boneCount; i++)
                {
                    rlDrawCube(anims[0].framePoses[animFrameCounter][i].translation, 0.2f, 0.2f, 0.2f, RED);
                }

                rlDrawGrid(10, 1.0f);         // Draw a grid

            rlEndMode3D();

            rlDrawText("PRESS SPACE to PLAY MODEL ANIMATION", 10, 10, 20, MAROON);
            rlDrawText("(c) Guy IQM 3D model by @culacant", screenWidth - 200, screenHeight - 20, 10, GRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);                     // Unload texture
    rlUnloadModelAnimations(anims, animsCount);   // Unload model animations data
    rlUnloadModel(model);                         // Unload model

    rlCloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
