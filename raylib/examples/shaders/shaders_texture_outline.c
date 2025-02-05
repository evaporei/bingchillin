/*******************************************************************************************
*
*   raylib [shaders] example - Apply an shdrOutline to a texture
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   Example originally created with raylib 4.0, last time updated with raylib 4.0
*
*   Example contributed by Samuel Skiff (@GoldenThumbs) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Samuel SKiff (@GoldenThumbs) and Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - Apply an outline to a texture");

    Texture2D texture = LoadTexture("resources/fudesumi.png");

    rlShader shdrOutline = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/outline.fs", GLSL_VERSION));

    float outlineSize = 2.0f;
    float outlineColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };     // Normalized RED color
    float textureSize[2] = { (float)texture.width, (float)texture.height };

    // Get shader locations
    int outlineSizeLoc = rlGetShaderLocation(shdrOutline, "outlineSize");
    int outlineColorLoc = rlGetShaderLocation(shdrOutline, "outlineColor");
    int textureSizeLoc = rlGetShaderLocation(shdrOutline, "textureSize");

    // Set shader values (they can be changed later)
    rlSetShaderValue(shdrOutline, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shdrOutline, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
    rlSetShaderValue(shdrOutline, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        outlineSize += rlGetMouseWheelMove();
        if (outlineSize < 1.0f) outlineSize = 1.0f;

        rlSetShaderValue(shdrOutline, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginShaderMode(shdrOutline);

                rlDrawTexture(texture, rlGetScreenWidth()/2 - texture.width/2, -30, WHITE);

            rlEndShaderMode();

            rlDrawText("rlShader-based\ntexture\noutline", 10, 10, 20, GRAY);
            rlDrawText("Scroll mouse wheel to\nchange outline size", 10, 72, 20, GRAY);
            rlDrawText(rlTextFormat("Outline size: %i px", (int)outlineSize), 10, 120, 20, MAROON);

            rlDrawFPS(710, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);
    rlUnloadShader(shdrOutline);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
