/*******************************************************************************************
*
*   raylib [shaders] example - rlTexture Waves
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.7
*
*   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture waves");

    // Load texture texture to apply shaders
    Texture2D texture = LoadTexture("resources/space.png");

    // Load shader and setup location points and values
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/wave.fs", GLSL_VERSION));

    int secondsLoc = rlGetShaderLocation(shader, "seconds");
    int freqXLoc = rlGetShaderLocation(shader, "freqX");
    int freqYLoc = rlGetShaderLocation(shader, "freqY");
    int ampXLoc = rlGetShaderLocation(shader, "ampX");
    int ampYLoc = rlGetShaderLocation(shader, "ampY");
    int speedXLoc = rlGetShaderLocation(shader, "speedX");
    int speedYLoc = rlGetShaderLocation(shader, "speedY");

    // rlShader uniform values that can be updated at any time
    float freqX = 25.0f;
    float freqY = 25.0f;
    float ampX = 5.0f;
    float ampY = 5.0f;
    float speedX = 8.0f;
    float speedY = 8.0f;

    float screenSize[2] = { (float)rlGetScreenWidth(), (float)rlGetScreenHeight() };
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "size"), &screenSize, SHADER_UNIFORM_VEC2);
    rlSetShaderValue(shader, freqXLoc, &freqX, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, freqYLoc, &freqY, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, ampXLoc, &ampX, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, ampYLoc, &ampY, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, speedXLoc, &speedX, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, speedYLoc, &speedY, SHADER_UNIFORM_FLOAT);

    float seconds = 0.0f;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // -------------------------------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        seconds += rlGetFrameTime();

        rlSetShaderValue(shader, secondsLoc, &seconds, SHADER_UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginShaderMode(shader);

                rlDrawTexture(texture, 0, 0, WHITE);
                rlDrawTexture(texture, texture.width, 0, WHITE);

            rlEndShaderMode();

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);         // Unload shader
    UnloadTexture(texture);       // Unload texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
