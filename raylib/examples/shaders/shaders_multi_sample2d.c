/*******************************************************************************************
*
*   raylib [shaders] example - Multiple sample2D with default batch system
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 3.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib - multiple sample2D");

    rlImage imRed = rlGenImageColor(800, 450, (rlColor){ 255, 0, 0, 255 });
    rlTexture texRed = rlLoadTextureFromImage(imRed);
    rlUnloadImage(imRed);

    rlImage imBlue = rlGenImageColor(800, 450, (rlColor){ 0, 0, 255, 255 });
    rlTexture texBlue = rlLoadTextureFromImage(imBlue);
    rlUnloadImage(imBlue);

    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/color_mix.fs", GLSL_VERSION));

    // Get an additional sampler2D location to be enabled on drawing
    int texBlueLoc = rlGetShaderLocation(shader, "texture1");

    // Get shader uniform for divider
    int dividerLoc = rlGetShaderLocation(shader, "divider");
    float dividerValue = 0.5f;

    rlSetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())                // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_RIGHT)) dividerValue += 0.01f;
        else if (rlIsKeyDown(KEY_LEFT)) dividerValue -= 0.01f;

        if (dividerValue < 0.0f) dividerValue = 0.0f;
        else if (dividerValue > 1.0f) dividerValue = 1.0f;

        rlSetShaderValue(shader, dividerLoc, &dividerValue, SHADER_UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginShaderMode(shader);

                // WARNING: Additional samplers are enabled for all draw calls in the batch,
                // rlEndShaderMode() forces batch drawing and consequently resets active textures
                // to let other sampler2D to be activated on consequent drawings (if required)
                rlSetShaderValueTexture(shader, texBlueLoc, texBlue);

                // We are drawing texRed using default sampler2D texture0 but
                // an additional texture units is enabled for texBlue (sampler2D texture1)
                rlDrawTexture(texRed, 0, 0, WHITE);

            rlEndShaderMode();

            rlDrawText("Use KEY_LEFT/KEY_RIGHT to move texture mixing in shader!", 80, rlGetScreenHeight() - 40, 20, RAYWHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);       // Unload shader
    UnloadTexture(texRed);      // Unload texture
    UnloadTexture(texBlue);     // Unload texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}