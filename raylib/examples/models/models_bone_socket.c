/*******************************************************************************************
*
*   raylib [core] example - Using bones as socket for calculating the positioning of something
* 
*   Example originally created with raylib 4.5, last time updated with raylib 4.5
*
*   Example contributed by iP (@ipzaur) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2024 iP (@ipzaur)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"

#define BONE_SOCKETS        3
#define BONE_SOCKET_HAT     0
#define BONE_SOCKET_HAND_R  1
#define BONE_SOCKET_HAND_L  2

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - bone socket");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 5.0f, 5.0f, 5.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };  // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };      // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    // Load gltf model
    rlModel characterModel = rlLoadModel("resources/models/gltf/greenman.glb"); // Load character model
    rlModel equipModel[BONE_SOCKETS] = {
        rlLoadModel("resources/models/gltf/greenman_hat.glb"),    // Index for the hat model is the same as BONE_SOCKET_HAT
        rlLoadModel("resources/models/gltf/greenman_sword.glb"),  // Index for the sword model is the same as BONE_SOCKET_HAND_R
        rlLoadModel("resources/models/gltf/greenman_shield.glb")  // Index for the shield model is the same as BONE_SOCKET_HAND_L
    };
    
    bool showEquip[3] = { true, true, true };   // Toggle on/off equip

    // Load gltf model animations
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    rlModelAnimation *modelAnimations = rlLoadModelAnimations("resources/models/gltf/greenman.glb", &animsCount);

    // indices of bones for sockets
    int boneSocketIndex[BONE_SOCKETS] = { -1, -1, -1 };

    // search bones for sockets 
    for (int i = 0; i < characterModel.boneCount; i++)
    {
        if (rlTextIsEqual(characterModel.bones[i].name, "socket_hat"))
        {
            boneSocketIndex[BONE_SOCKET_HAT] = i;
            continue;
        }
        
        if (rlTextIsEqual(characterModel.bones[i].name, "socket_hand_R"))
        {
            boneSocketIndex[BONE_SOCKET_HAND_R] = i;
            continue;
        }
        
        if (rlTextIsEqual(characterModel.bones[i].name, "socket_hand_L"))
        {
            boneSocketIndex[BONE_SOCKET_HAND_L] = i;
            continue;
        }
    }

    rlVector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position
    unsigned short angle = 0;           // Set angle for rotate character

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_THIRD_PERSON);
        
        // Rotate character
        if (rlIsKeyDown(KEY_F)) angle = (angle + 1)%360;
        else if (rlIsKeyDown(KEY_H)) angle = (360 + angle - 1)%360;

        // Select current animation
        if (rlIsKeyPressed(KEY_T)) animIndex = (animIndex + 1)%animsCount;
        else if (rlIsKeyPressed(KEY_G)) animIndex = (animIndex + animsCount - 1)%animsCount;

        // Toggle shown of equip
        if (rlIsKeyPressed(KEY_ONE)) showEquip[BONE_SOCKET_HAT] = !showEquip[BONE_SOCKET_HAT];
        if (rlIsKeyPressed(KEY_TWO)) showEquip[BONE_SOCKET_HAND_R] = !showEquip[BONE_SOCKET_HAND_R];
        if (rlIsKeyPressed(KEY_THREE)) showEquip[BONE_SOCKET_HAND_L] = !showEquip[BONE_SOCKET_HAND_L];
        
        // Update model animation
        rlModelAnimation anim = modelAnimations[animIndex];
        animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
        rlUpdateModelAnimation(characterModel, anim, animCurrentFrame);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
                // Draw character
                Quaternion characterRotate = QuaternionFromAxisAngle((rlVector3){ 0.0f, 1.0f, 0.0f }, angle*DEG2RAD);
                characterModel.transform = MatrixMultiply(QuaternionToMatrix(characterRotate), MatrixTranslate(position.x, position.y, position.z));
                rlUpdateModelAnimation(characterModel, anim, animCurrentFrame);
                rlDrawMesh(characterModel.meshes[0], characterModel.materials[1], characterModel.transform);

                // Draw equipments (hat, sword, shield)
                for (int i = 0; i < BONE_SOCKETS; i++)
                {
                    if (!showEquip[i]) continue;

                    rlTransform *transform = &anim.framePoses[animCurrentFrame][boneSocketIndex[i]];
                    Quaternion inRotation = characterModel.bindPose[boneSocketIndex[i]].rotation;
                    Quaternion outRotation = transform->rotation;
                    
                    // Calculate socket rotation (angle between bone in initial pose and same bone in current animation frame)
                    Quaternion rotate = QuaternionMultiply(outRotation, QuaternionInvert(inRotation));
                    rlMatrix matrixTransform = QuaternionToMatrix(rotate);
                    // Translate socket to its position in the current animation
                    matrixTransform = MatrixMultiply(matrixTransform, MatrixTranslate(transform->translation.x, transform->translation.y, transform->translation.z));
                    // rlTransform the socket using the transform of the character (angle and translate)
                    matrixTransform = MatrixMultiply(matrixTransform, characterModel.transform);
                    
                    // Draw mesh at socket position with socket angle rotation
                    rlDrawMesh(equipModel[i].meshes[0], equipModel[i].materials[1], matrixTransform);
                }

                rlDrawGrid(10, 1.0f);
            rlEndMode3D();

            rlDrawText("Use the T/G to switch animation", 10, 10, 20, GRAY);
            rlDrawText("Use the F/H to rotate character left/right", 10, 35, 20, GRAY);
            rlDrawText("Use the 1,2,3 to toggle shown of hat, sword and shield", 10, 60, 20, GRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModelAnimations(modelAnimations, animsCount);
    rlUnloadModel(characterModel);         // Unload character model and meshes/material
    
    // Unload equipment model and meshes/material
    for (int i = 0; i < BONE_SOCKETS; i++) rlUnloadModel(equipModel[i]);

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}