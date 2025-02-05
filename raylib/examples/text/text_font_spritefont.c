/*******************************************************************************************
*
*   raylib [text] example - Sprite font loading
*
*   NOTE: Sprite fonts should be generated following this conventions:
*
*     - Characters must be ordered starting with character 32 (Space)
*     - Every character must be contained within the same rlRectangle height
*     - Every character and every line must be separated by the same distance (margin/padding)
*     - Rectangles must be defined by a MAGENTA color background
*
*   Following those constraints, a font can be provided just by an image,
*   this is quite handy to avoid additional font descriptor files (like BMFonts use).
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - sprite font loading");

    const char msg1[50] = "THIS IS A custom SPRITE FONT...";
    const char msg2[50] = "...and this is ANOTHER CUSTOM font...";
    const char msg3[50] = "...and a THIRD one! GREAT! :D";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)
    rlFont font1 = rlLoadFont("resources/custom_mecha.png");          // rlFont loading
    rlFont font2 = rlLoadFont("resources/custom_alagard.png");        // rlFont loading
    rlFont font3 = rlLoadFont("resources/custom_jupiter_crash.png");  // rlFont loading

    rlVector2 fontPosition1 = { screenWidth/2.0f - rlMeasureTextEx(font1, msg1, (float)font1.baseSize, -3).x/2,
                              screenHeight/2.0f - font1.baseSize/2.0f - 80.0f };

    rlVector2 fontPosition2 = { screenWidth/2.0f - rlMeasureTextEx(font2, msg2, (float)font2.baseSize, -2.0f).x/2.0f,
                              screenHeight/2.0f - font2.baseSize/2.0f - 10.0f };

    rlVector2 fontPosition3 = { screenWidth/2.0f - rlMeasureTextEx(font3, msg3, (float)font3.baseSize, 2.0f).x/2.0f,
                              screenHeight/2.0f - font3.baseSize/2.0f + 50.0f };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update variables here...
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawTextEx(font1, msg1, fontPosition1, (float)font1.baseSize, -3, WHITE);
            rlDrawTextEx(font2, msg2, fontPosition2, (float)font2.baseSize, -2, WHITE);
            rlDrawTextEx(font3, msg3, fontPosition3, (float)font3.baseSize, 2, WHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadFont(font1);      // rlFont unloading
    rlUnloadFont(font2);      // rlFont unloading
    rlUnloadFont(font3);      // rlFont unloading

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}