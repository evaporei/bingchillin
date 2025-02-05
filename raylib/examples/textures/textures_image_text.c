/*******************************************************************************************
*
*   raylib [texture] example - rlImage text drawing using TTF generated font
*
*   Example originally created with raylib 1.8, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [texture] example - image text drawing");

    rlImage parrots = rlLoadImage("resources/parrots.png"); // Load image in CPU memory (RAM)

    // TTF rlFont loading with custom generation parameters
    rlFont font = rlLoadFontEx("resources/KAISG.ttf", 64, 0, 0);

    // Draw over image using custom font
    rlImageDrawTextEx(&parrots, font, "[Parrots font drawing]", (rlVector2){ 20.0f, 20.0f }, (float)font.baseSize, 0.0f, RED);

    Texture2D texture = rlLoadTextureFromImage(parrots);  // rlImage converted to texture, uploaded to GPU memory (VRAM)
    rlUnloadImage(parrots);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    rlVector2 position = { (float)(screenWidth/2 - texture.width/2), (float)(screenHeight/2 - texture.height/2 - 20) };

    bool showFont = false;

    rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_SPACE)) showFont = true;
        else showFont = false;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            if (!showFont)
            {
                // Draw texture with text already drawn inside
                rlDrawTextureV(texture, position, WHITE);

                // Draw text directly using sprite font
                rlDrawTextEx(font, "[Parrots font drawing]", (rlVector2){ position.x + 20,
                           position.y + 20 + 280 }, (float)font.baseSize, 0.0f, WHITE);
            }
            else rlDrawTexture(font.texture, screenWidth/2 - font.texture.width/2, 50, BLACK);

            rlDrawText("PRESS SPACE to SHOW FONT ATLAS USED", 290, 420, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);     // rlTexture unloading

    rlUnloadFont(font);           // Unload custom font

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}