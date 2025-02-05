/*******************************************************************************************
*
*   raylib [shaders] example - basic lighting
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example originally created with raylib 3.0, last time updated with raylib 4.2
*
*   Example contributed by Chris Camacho (@codifies) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Chris Camacho (@codifies) and Ramon Santamaria (@raysan5)
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
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - basic lighting");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 2.0f, 4.0f, 6.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.5f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load basic lighting shader
    rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               rlTextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading, 
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = rlGetShaderLocation(shader, "matModel");
    
    // Ambient light level (some basic lighting)
    int ambientLoc = rlGetShaderLocation(shader, "ambient");
    rlSetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Create lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (rlVector3){ -2, 1, -2 }, Vector3Zero(), YELLOW, shader);
    lights[1] = CreateLight(LIGHT_POINT, (rlVector3){ 2, 1, 2 }, Vector3Zero(), RED, shader);
    lights[2] = CreateLight(LIGHT_POINT, (rlVector3){ -2, 1, 2 }, Vector3Zero(), GREEN, shader);
    lights[3] = CreateLight(LIGHT_POINT, (rlVector3){ 2, 1, -2 }, Vector3Zero(), BLUE, shader);

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        rlSetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        
        // Check key inputs to enable/disable lights
        if (rlIsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
        if (rlIsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
        if (rlIsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
        if (rlIsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }
        
        // Update light values (actually, only enable/disable them)
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlBeginShaderMode(shader);

                    rlDrawPlane(Vector3Zero(), (rlVector2) { 10.0, 10.0 }, WHITE);
                    rlDrawCube(Vector3Zero(), 2.0, 4.0, 2.0, WHITE);

                rlEndShaderMode();

                // Draw spheres to show where the lights are
                for (int i = 0; i < MAX_LIGHTS; i++)
                {
                    if (lights[i].enabled) rlDrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                    else rlDrawSphereWires(lights[i].position, 0.2f, 8, 8, rlColorAlpha(lights[i].color, 0.3f));
                }

                rlDrawGrid(10, 1.0f);

            rlEndMode3D();

            rlDrawFPS(10, 10);

            rlDrawText("Use keys [Y][R][G][B] to toggle lights", 10, 40, 20, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);   // Unload shader

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

