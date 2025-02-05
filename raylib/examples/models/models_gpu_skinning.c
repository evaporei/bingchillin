/*******************************************************************************************
*
*   raylib [core] example - Doing skinning on the gpu using a vertex shader
* 
*   Example originally created with raylib 4.5, last time updated with raylib 4.5
*
*   Example contributed by Daniel Holden (@orangeduck) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2024 Daniel Holden (@orangeduck)
* 
*   Note: Due to limitations in the Apple OpenGL driver, this feature does not work on MacOS
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - GPU skinning");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 5.0f, 5.0f, 5.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };  // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };      // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    // Load gltf model
    rlModel characterModel = rlLoadModel("resources/models/gltf/greenman.glb"); // Load character model
    
    // Load skinning shader
    rlShader skinningShader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/skinning.vs", GLSL_VERSION),
                                       rlTextFormat("resources/shaders/glsl%i/skinning.fs", GLSL_VERSION));
    
    characterModel.materials[1].shader = skinningShader;
    
    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    rlModelAnimation *modelAnimations = rlLoadModelAnimations("resources/models/gltf/greenman.glb", &animsCount);

    rlVector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_THIRD_PERSON);
        
        // Select current animation
        if (rlIsKeyPressed(KEY_T)) animIndex = (animIndex + 1)%animsCount;
        else if (rlIsKeyPressed(KEY_G)) animIndex = (animIndex + animsCount - 1)%animsCount;

        // Update model animation
        rlModelAnimation anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
        rlUpdateModelAnimationBoneMatrices(characterModel, anim, animCurrentFrame);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
            
                // Draw character
                characterModel.transform = MatrixTranslate(position.x, position.y, position.z);
                rlUpdateModelAnimationBoneMatrices(characterModel, anim, animCurrentFrame);
                rlDrawMesh(characterModel.meshes[0], characterModel.materials[1], characterModel.transform);

                rlDrawGrid(10, 1.0f);
            rlEndMode3D();

            rlDrawText("Use the T/G to switch animation", 10, 10, 20, GRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModelAnimations(modelAnimations, animsCount);
    rlUnloadModel(characterModel);         // Unload character model and meshes/material
    rlUnloadShader(skinningShader);
    
    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}