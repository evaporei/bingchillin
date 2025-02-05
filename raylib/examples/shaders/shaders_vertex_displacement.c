/*******************************************************************************************
*
*   raylib [shaders] example - Vertex displacement
*
*   Example originally created with raylib 5.0, last time updated with raylib 4.5
*
*   Example contributed by <Alex ZH> (@ZzzhHe) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 <Alex ZH> (@ZzzhHe)
*
********************************************************************************************/

#include "raylib.h"

#include "rlgl.h"

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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - vertex displacement");

    // set up camera
    Camera camera = {0};
    camera.position = (rlVector3) {20.0f, 5.0f, -20.0f};
    camera.target = (rlVector3) {0.0f, 0.0f, 0.0f};
    camera.up = (rlVector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Load vertex and fragment shaders
    rlShader shader = rlLoadShader(
        rlTextFormat("resources/shaders/glsl%i/vertex_displacement.vs", GLSL_VERSION),
        rlTextFormat("resources/shaders/glsl%i/vertex_displacement.fs", GLSL_VERSION));
    
    // Load perlin noise texture
    rlImage perlinNoiseImage = rlGenImagePerlinNoise(512, 512, 0, 0, 1.0f);
    rlTexture perlinNoiseMap = rlLoadTextureFromImage(perlinNoiseImage);
    rlUnloadImage(perlinNoiseImage);

    // Set shader uniform location
    int perlinNoiseMapLoc = rlGetShaderLocation(shader, "perlinNoiseMap");
    rlEnableShader(shader.id);
    rlActiveTextureSlot(1);
    rlEnableTexture(perlinNoiseMap.id);
    rlSetUniformSampler(perlinNoiseMapLoc, 1);
    
    // Create a plane mesh and model
    rlMesh planeMesh = rlGenMeshPlane(50, 50, 50, 50);
    rlModel planeModel = rlLoadModelFromMesh(planeMesh);
    // Set plane model material
    planeModel.materials[0].shader = shader;

    float time = 0.0f;

    rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FREE); // Update camera

        time += rlGetFrameTime(); // Update time variable
        rlSetShaderValue(shader, rlGetShaderLocation(shader, "time"), &time, SHADER_UNIFORM_FLOAT); // Send time value to shader

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlBeginShaderMode(shader);
                    // Draw plane model
                    rlDrawModel(planeModel, (rlVector3){ 0.0f, 0.0f, 0.0f }, 1.0f, (rlColor) {255, 255, 255, 255});
                rlEndShaderMode();

            rlEndMode3D();

            rlDrawText("Vertex displacement", 10, 10, 20, DARKGRAY);
            rlDrawFPS(10, 40);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);
    rlUnloadModel(planeModel);
    UnloadTexture(perlinNoiseMap);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
