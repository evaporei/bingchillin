/*******************************************************************************************
*
*   raylib [text] example - raylib fonts loading
*
*   NOTE: raylib is distributed with some free to use fonts (even for commercial pourposes!)
*         To view details and credits for those fonts, check raylib license file
*
*   Example originally created with raylib 1.7, last time updated with raylib 3.7
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_FONTS   8

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - raylib fonts");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    rlFont fonts[MAX_FONTS] = { 0 };

    fonts[0] = rlLoadFont("resources/fonts/alagard.png");
    fonts[1] = rlLoadFont("resources/fonts/pixelplay.png");
    fonts[2] = rlLoadFont("resources/fonts/mecha.png");
    fonts[3] = rlLoadFont("resources/fonts/setback.png");
    fonts[4] = rlLoadFont("resources/fonts/romulus.png");
    fonts[5] = rlLoadFont("resources/fonts/pixantiqua.png");
    fonts[6] = rlLoadFont("resources/fonts/alpha_beta.png");
    fonts[7] = rlLoadFont("resources/fonts/jupiter_crash.png");

    const char *messages[MAX_FONTS] = { "ALAGARD FONT designed by Hewett Tsoi",
                                "PIXELPLAY FONT designed by Aleksander Shevchuk",
                                "MECHA FONT designed by Captain Falcon",
                                "SETBACK FONT designed by Brian Kent (AEnigma)",
                                "ROMULUS FONT designed by Hewett Tsoi",
                                "PIXANTIQUA FONT designed by Gerhard Grossmann",
                                "ALPHA_BETA FONT designed by Brian Kent (AEnigma)",
                                "JUPITER_CRASH FONT designed by Brian Kent (AEnigma)" };

    const int spacings[MAX_FONTS] = { 2, 4, 8, 4, 3, 4, 4, 1 };

    rlVector2 positions[MAX_FONTS] = { 0 };

    for (int i = 0; i < MAX_FONTS; i++)
    {
        positions[i].x = screenWidth/2.0f - rlMeasureTextEx(fonts[i], messages[i], fonts[i].baseSize*2.0f, (float)spacings[i]).x/2.0f;
        positions[i].y = 60.0f + fonts[i].baseSize + 45.0f*i;
    }

    // Small Y position corrections
    positions[3].y += 8;
    positions[4].y += 2;
    positions[7].y -= 8;

    rlColor colors[MAX_FONTS] = { MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, LIME, GOLD, RED };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("free fonts included with raylib", 250, 20, 20, DARKGRAY);
            rlDrawLine(220, 50, 590, 50, DARKGRAY);

            for (int i = 0; i < MAX_FONTS; i++)
            {
                rlDrawTextEx(fonts[i], messages[i], positions[i], fonts[i].baseSize*2.0f, (float)spacings[i], colors[i]);
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // Fonts unloading
    for (int i = 0; i < MAX_FONTS; i++) rlUnloadFont(fonts[i]);

    rlCloseWindow();                 // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}