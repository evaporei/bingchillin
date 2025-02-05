/*******************************************************************************************
*
*   raylib [shaders] example - fog
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.7
*
*   Example contributed by Chris Camacho (@chriscamacho) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Chris Camacho (@chriscamacho) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

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

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - fog");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 2.0f, 2.0f, 6.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.5f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load models and texture
    rlModel modelA = rlLoadModelFromMesh(rlGenMeshTorus(0.4f, 1.0f, 16, 32));
    rlModel modelB = rlLoadModelFromMesh(rlGenMeshCube(1.0f, 1.0f, 1.0f));
    rlModel modelC = rlLoadModelFromMesh(rlGenMeshSphere(0.5f, 32, 32));
    rlTexture texture = LoadTexture("resources/texel_checker.png");

    // Assign texture to default model material
    modelA.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    modelB.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    modelC.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Load shader and set up some uniforms
    rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               rlTextFormat("resources/shaders/glsl%i/fog.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_MATRIX_MODEL] = rlGetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shader, "viewPos");

    // Ambient light level
    int ambientLoc = rlGetShaderLocation(shader, "ambient");
    rlSetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);

    float fogDensity = 0.15f;
    int fogDensityLoc = rlGetShaderLocation(shader, "fogDensity");
    rlSetShaderValue(shader, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);

    // NOTE: All models share the same shader
    modelA.materials[0].shader = shader;
    modelB.materials[0].shader = shader;
    modelC.materials[0].shader = shader;

    // Using just 1 point lights
    CreateLight(LIGHT_POINT, (rlVector3){ 0, 2, 6 }, Vector3Zero(), WHITE, shader);

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        if (rlIsKeyDown(KEY_UP))
        {
            fogDensity += 0.001f;
            if (fogDensity > 1.0f) fogDensity = 1.0f;
        }

        if (rlIsKeyDown(KEY_DOWN))
        {
            fogDensity -= 0.001f;
            if (fogDensity < 0.0f) fogDensity = 0.0f;
        }

        rlSetShaderValue(shader, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);

        // Rotate the torus
        modelA.transform = MatrixMultiply(modelA.transform, MatrixRotateX(-0.025f));
        modelA.transform = MatrixMultiply(modelA.transform, MatrixRotateZ(0.012f));

        // Update the light shader with the camera view position
        rlSetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(GRAY);

            rlBeginMode3D(camera);

                // Draw the three models
                rlDrawModel(modelA, Vector3Zero(), 1.0f, WHITE);
                rlDrawModel(modelB, (rlVector3){ -2.6f, 0, 0 }, 1.0f, WHITE);
                rlDrawModel(modelC, (rlVector3){ 2.6f, 0, 0 }, 1.0f, WHITE);

                for (int i = -20; i < 20; i += 2) rlDrawModel(modelA,(rlVector3){ (float)i, 0, 2 }, 1.0f, WHITE);

            rlEndMode3D();

            rlDrawText(rlTextFormat("Use KEY_UP/KEY_DOWN to change fog density [%.2f]", fogDensity), 10, 10, 20, RAYWHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(modelA);        // Unload the model A
    rlUnloadModel(modelB);        // Unload the model B
    rlUnloadModel(modelC);        // Unload the model C
    UnloadTexture(texture);     // Unload the texture
    rlUnloadShader(shader);       // Unload shader

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
