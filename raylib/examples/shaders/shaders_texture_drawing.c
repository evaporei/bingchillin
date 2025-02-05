/*******************************************************************************************
*
*   raylib [textures] example - rlTexture drawing
*
*   NOTE: This example illustrates how to draw into a blank texture using a shader
*
*   Example originally created with raylib 2.0, last time updated with raylib 3.7
*
*   Example contributed by Michał Ciesielski and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Michał Ciesielski and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture drawing");

    rlImage imBlank = rlGenImageColor(1024, 1024, BLANK);
    Texture2D texture = rlLoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
    rlUnloadImage(imBlank);

    // NOTE: Using GLSL 330 shader version, on OpenGL ES 2.0 use GLSL 100 shader version
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/cubes_panning.fs", GLSL_VERSION));

    float time = 0.0f;
    int timeLoc = rlGetShaderLocation(shader, "uTime");
    rlSetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // -------------------------------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        time = (float)rlGetTime();
        rlSetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginShaderMode(shader);    // Enable our custom shader for next shapes/textures drawings
                rlDrawTexture(texture, 0, 0, WHITE);  // Drawing BLANK texture, all magic happens on shader
            rlEndShaderMode();            // Disable our custom shader, return to default shader

            rlDrawText("BACKGROUND is PAINTED and ANIMATED on SHADER!", 10, 10, 20, MAROON);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);
    UnloadTexture(texture);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
