/*******************************************************************************************
*
*   raylib [text] example - rlFont SDF loading
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include <stdlib.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - SDF fonts");

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    const char msg[50] = "Signed Distance Fields";

    // Loading file to memory
    int fileSize = 0;
    unsigned char *fileData = rlLoadFileData("resources/anonymous_pro_bold.ttf", &fileSize);

    // Default font generation from TTF font
    rlFont fontDefault = { 0 };
    fontDefault.baseSize = 16;
    fontDefault.glyphCount = 95;

    // Loading font data from memory data
    // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 95 (autogenerate chars array)
    fontDefault.glyphs = rlLoadFontData(fileData, fileSize, 16, 0, 95, FONT_DEFAULT);
    // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 4 px, pack method: 0 (default)
    rlImage atlas = rlGenImageFontAtlas(fontDefault.glyphs, &fontDefault.recs, 95, 16, 4, 0);
    fontDefault.texture = rlLoadTextureFromImage(atlas);
    rlUnloadImage(atlas);

    // SDF font generation from TTF font
    rlFont fontSDF = { 0 };
    fontSDF.baseSize = 16;
    fontSDF.glyphCount = 95;
    // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 0 (defaults to 95)
    fontSDF.glyphs = rlLoadFontData(fileData, fileSize, 16, 0, 0, FONT_SDF);
    // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 0 px, pack method: 1 (Skyline algorythm)
    atlas = rlGenImageFontAtlas(fontSDF.glyphs, &fontSDF.recs, 95, 16, 0, 1);
    fontSDF.texture = rlLoadTextureFromImage(atlas);
    rlUnloadImage(atlas);

    rlUnloadFileData(fileData);      // Free memory from loaded file

    // Load SDF required shader (we use default vertex shader)
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/sdf.fs", GLSL_VERSION));
    rlSetTextureFilter(fontSDF.texture, TEXTURE_FILTER_BILINEAR);    // Required for SDF font

    rlVector2 fontPosition = { 40, screenHeight/2.0f - 50 };
    rlVector2 textSize = { 0.0f, 0.0f };
    float fontSize = 16.0f;
    int currentFont = 0;            // 0 - fontDefault, 1 - fontSDF

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        fontSize += rlGetMouseWheelMove()*8.0f;

        if (fontSize < 6) fontSize = 6;

        if (rlIsKeyDown(KEY_SPACE)) currentFont = 1;
        else currentFont = 0;

        if (currentFont == 0) textSize = rlMeasureTextEx(fontDefault, msg, fontSize, 0);
        else textSize = rlMeasureTextEx(fontSDF, msg, fontSize, 0);

        fontPosition.x = rlGetScreenWidth()/2 - textSize.x/2;
        fontPosition.y = rlGetScreenHeight()/2 - textSize.y/2 + 80;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            if (currentFont == 1)
            {
                // NOTE: SDF fonts require a custom SDf shader to compute fragment color
                rlBeginShaderMode(shader);    // Activate SDF font shader
                    rlDrawTextEx(fontSDF, msg, fontPosition, fontSize, 0, BLACK);
                rlEndShaderMode();            // Activate our default shader for next drawings

                rlDrawTexture(fontSDF.texture, 10, 10, BLACK);
            }
            else
            {
                rlDrawTextEx(fontDefault, msg, fontPosition, fontSize, 0, BLACK);
                rlDrawTexture(fontDefault.texture, 10, 10, BLACK);
            }

            if (currentFont == 1) rlDrawText("SDF!", 320, 20, 80, RED);
            else rlDrawText("default font", 315, 40, 30, GRAY);

            rlDrawText("FONT SIZE: 16.0", rlGetScreenWidth() - 240, 20, 20, DARKGRAY);
            rlDrawText(rlTextFormat("RENDER SIZE: %02.02f", fontSize), rlGetScreenWidth() - 240, 50, 20, DARKGRAY);
            rlDrawText("Use MOUSE WHEEL to SCALE TEXT!", rlGetScreenWidth() - 240, 90, 10, DARKGRAY);

            rlDrawText("HOLD SPACE to USE SDF FONT VERSION!", 340, rlGetScreenHeight() - 30, 20, MAROON);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadFont(fontDefault);    // Default font unloading
    rlUnloadFont(fontSDF);        // SDF font unloading

    rlUnloadShader(shader);       // Unload SDF shader

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}