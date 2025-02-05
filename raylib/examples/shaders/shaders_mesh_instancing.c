/*******************************************************************************************
*
*   raylib [shaders] example - rlMesh instancing
*
*   Example originally created with raylib 3.7, last time updated with raylib 4.2
*
*   Example contributed by @seanpringle and reviewed by Max (@moliad) and Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 @seanpringle, Max (@moliad) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/


#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include <stdlib.h>         // Required for: calloc(), free()

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_INSTANCES  10000

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - mesh instancing");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ -125.0f, 125.0f, -125.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };              // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };                  // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                        // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                     // Camera projection type

    // Define mesh to be instanced
    rlMesh cube = rlGenMeshCube(1.0f, 1.0f, 1.0f);

    // Define transforms to be uploaded to GPU for instances
    rlMatrix *transforms = (rlMatrix *)RL_CALLOC(MAX_INSTANCES, sizeof(rlMatrix));   // Pre-multiplied transformations passed to rlgl

    // Translate and rotate cubes randomly
    for (int i = 0; i < MAX_INSTANCES; i++)
    {
        rlMatrix translation = MatrixTranslate((float)rlGetRandomValue(-50, 50), (float)rlGetRandomValue(-50, 50), (float)rlGetRandomValue(-50, 50));
        rlVector3 axis = Vector3Normalize((rlVector3){ (float)rlGetRandomValue(0, 360), (float)rlGetRandomValue(0, 360), (float)rlGetRandomValue(0, 360) });
        float angle = (float)rlGetRandomValue(0, 10)*DEG2RAD;
        rlMatrix rotation = MatrixRotate(axis, angle);
        
        transforms[i] = MatrixMultiply(rotation, translation);
    }

    // Load lighting shader
    rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/lighting_instancing.vs", GLSL_VERSION),
                               rlTextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    // Get shader locations
    shader.locs[SHADER_LOC_MATRIX_MVP] = rlGetShaderLocation(shader, "mvp");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = rlGetShaderLocationAttrib(shader, "instanceTransform");

    // Set shader value: ambient light level
    int ambientLoc = rlGetShaderLocation(shader, "ambient");
    rlSetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Create one light
    CreateLight(LIGHT_DIRECTIONAL, (rlVector3){ 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, shader);

    // NOTE: We are assigning the intancing shader to material.shader
    // to be used on mesh drawing with rlDrawMeshInstanced()
    rlMaterial matInstances = rlLoadMaterialDefault();
    matInstances.shader = shader;
    matInstances.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    // Load default material (using raylib intenral default shader) for non-instanced mesh drawing
    // WARNING: Default shader enables vertex color attribute BUT rlGenMeshCube() does not generate vertex colors, so,
    // when drawing the color attribute is disabled and a default color value is provided as input for thevertex attribute
    rlMaterial matDefault = rlLoadMaterialDefault();
    matDefault.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        // Update the light shader with the camera view position
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        rlSetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                // Draw cube mesh with default material (BLUE)
                rlDrawMesh(cube, matDefault, MatrixTranslate(-10.0f, 0.0f, 0.0f));

                // Draw meshes instanced using material containing instancing shader (RED + lighting),
                // transforms[] for the instances should be provided, they are dynamically
                // updated in GPU every frame, so we can animate the different mesh instances
                rlDrawMeshInstanced(cube, matInstances, transforms, MAX_INSTANCES);

                // Draw cube mesh with default material (BLUE)
                rlDrawMesh(cube, matDefault, MatrixTranslate(10.0f, 0.0f, 0.0f));

            rlEndMode3D();

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    RL_FREE(transforms);    // Free transforms

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
