/*******************************************************************************************
*
*   raylib [text] example - rlFont loading
*
*   NOTE: raylib can load fonts from multiple input file formats:
*
*     - TTF/OTF > Sprite font atlas is generated on loading, user can configure
*                 some of the generation parameters (size, characters to include)
*     - BMFonts > Angel code font fileformat, sprite font image must be provided
*                 together with the .fnt file, font generation cna not be configured
*     - XNA Spritefont > Sprite font image, following XNA Spritefont conventions,
*                 Characters in image must follow some spacing and order rules
*
*   Example originally created with raylib 1.4, last time updated with raylib 3.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - font loading");

    // Define characters to draw
    // NOTE: raylib supports UTF-8 encoding, following list is actually codified as UTF8 internally
    const char msg[256] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    // BMFont (AngelCode) : rlFont data and image atlas have been generated using external program
    rlFont fontBm = rlLoadFont("resources/pixantiqua.fnt");

    // TTF font : rlFont data and atlas are generated directly from TTF
    // NOTE: We define a font base size of 32 pixels tall and up-to 250 characters
    rlFont fontTtf = rlLoadFontEx("resources/pixantiqua.ttf", 32, 0, 250);

    rlSetTextLineSpacing(16);         // Set line spacing for multiline text (when line breaks are included '\n')

    bool useTtf = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_SPACE)) useTtf = true;
        else useTtf = false;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("Hold SPACE to use TTF generated font", 20, 20, 20, LIGHTGRAY);

            if (!useTtf)
            {
                rlDrawTextEx(fontBm, msg, (rlVector2){ 20.0f, 100.0f }, (float)fontBm.baseSize, 2, MAROON);
                rlDrawText("Using BMFont (Angelcode) imported", 20, rlGetScreenHeight() - 30, 20, GRAY);
            }
            else
            {
                rlDrawTextEx(fontTtf, msg, (rlVector2){ 20.0f, 100.0f }, (float)fontTtf.baseSize, 2, LIME);
                rlDrawText("Using TTF font generated", 20, rlGetScreenHeight() - 30, 20, GRAY);
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadFont(fontBm);     // AngelCode rlFont unloading
    rlUnloadFont(fontTtf);    // TTF rlFont unloading

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}