/*******************************************************************************************
*
*   raylib [textures] example - blend modes
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   Example originally created with raylib 3.5, last time updated with raylib 3.5
*
*   Example contributed by Karlo Licudine (@accidentalrebel) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 Karlo Licudine (@accidentalrebel)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - blend modes");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    rlImage bgImage = rlLoadImage("resources/cyberpunk_street_background.png");     // Loaded in CPU memory (RAM)
    Texture2D bgTexture = rlLoadTextureFromImage(bgImage);          // rlImage converted to texture, GPU memory (VRAM)

    rlImage fgImage = rlLoadImage("resources/cyberpunk_street_foreground.png");     // Loaded in CPU memory (RAM)
    Texture2D fgTexture = rlLoadTextureFromImage(fgImage);          // rlImage converted to texture, GPU memory (VRAM)

    // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    rlUnloadImage(bgImage);
    rlUnloadImage(fgImage);

    const int blendCountMax = 4;
    BlendMode blendMode = 0;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE))
        {
            if (blendMode >= (blendCountMax - 1)) blendMode = 0;
            else blendMode++;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawTexture(bgTexture, screenWidth/2 - bgTexture.width/2, screenHeight/2 - bgTexture.height/2, WHITE);

            // Apply the blend mode and then draw the foreground texture
            rlBeginBlendMode(blendMode);
                rlDrawTexture(fgTexture, screenWidth/2 - fgTexture.width/2, screenHeight/2 - fgTexture.height/2, WHITE);
            rlEndBlendMode();

            // Draw the texts
            rlDrawText("Press SPACE to change blend modes.", 310, 350, 10, GRAY);

            switch (blendMode)
            {
                case BLEND_ALPHA: rlDrawText("Current: BLEND_ALPHA", (screenWidth / 2) - 60, 370, 10, GRAY); break;
                case BLEND_ADDITIVE: rlDrawText("Current: BLEND_ADDITIVE", (screenWidth / 2) - 60, 370, 10, GRAY); break;
                case BLEND_MULTIPLIED: rlDrawText("Current: BLEND_MULTIPLIED", (screenWidth / 2) - 60, 370, 10, GRAY); break;
                case BLEND_ADD_COLORS: rlDrawText("Current: BLEND_ADD_COLORS", (screenWidth / 2) - 60, 370, 10, GRAY); break;
                default: break;
            }

            rlDrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", screenWidth - 330, screenHeight - 20, 10, GRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(fgTexture); // Unload foreground texture
    UnloadTexture(bgTexture); // Unload background texture

    rlCloseWindow();            // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
