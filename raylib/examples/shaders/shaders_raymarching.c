/*******************************************************************************************
*
*   raylib [shaders] example - Raymarching shapes generation
*
*   NOTE: This example requires raylib OpenGL 3.3 for shaders support and only #version 330
*         is currently supported. OpenGL ES 2.0 platforms are not supported at the moment.
*
*   Example originally created with raylib 2.0, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2018-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB -> Not supported at this moment
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

    rlSetConfigFlags(FLAG_WINDOW_RESIZABLE);
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - raymarching shapes");

    Camera camera = { 0 };
    camera.position = (rlVector3){ 2.5f, 2.5f, 3.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.7f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 65.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load raymarching shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/raymarching.fs", GLSL_VERSION));

    // Get shader locations for required uniforms
    int viewEyeLoc = rlGetShaderLocation(shader, "viewEye");
    int viewCenterLoc = rlGetShaderLocation(shader, "viewCenter");
    int runTimeLoc = rlGetShaderLocation(shader, "runTime");
    int resolutionLoc = rlGetShaderLocation(shader, "resolution");

    float resolution[2] = { (float)screenWidth, (float)screenHeight };
    rlSetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);

    float runTime = 0.0f;

    rlDisableCursor();                    // Limit cursor to relative movement inside the window
    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        float cameraTarget[3] = { camera.target.x, camera.target.y, camera.target.z };

        float deltaTime = rlGetFrameTime();
        runTime += deltaTime;

        // Set shader required uniform values
        rlSetShaderValue(shader, viewEyeLoc, cameraPos, SHADER_UNIFORM_VEC3);
        rlSetShaderValue(shader, viewCenterLoc, cameraTarget, SHADER_UNIFORM_VEC3);
        rlSetShaderValue(shader, runTimeLoc, &runTime, SHADER_UNIFORM_FLOAT);

        // Check if screen is resized
        if (rlIsWindowResized())
        {
            resolution[0] = (float)rlGetScreenWidth();
            resolution[1] = (float)rlGetScreenHeight();
            rlSetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            // We only draw a white full-screen rectangle,
            // frame is generated in shader using raymarching
            rlBeginShaderMode(shader);
                rlDrawRectangle(0, 0, rlGetScreenWidth(), rlGetScreenHeight(), WHITE);
            rlEndShaderMode();

            rlDrawText("(c) Raymarching shader by Iñigo Quilez. MIT License.", rlGetScreenWidth() - 280, rlGetScreenHeight() - 20, 10, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);           // Unload shader

    rlCloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
