/*******************************************************************************************
*
*   raylib [text] example - rlFont filters
*
*   NOTE: After font loading, font texture atlas filter could be configured for a softer
*   display of the font when scaling it to different sizes, that way, it's not required
*   to generate multiple fonts at multiple sizes (as long as the scaling is not very different)
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - font filters");

    const char msg[50] = "Loaded rlFont";

    // NOTE: Textures/Fonts MUST be loaded after Window initialization (OpenGL context is required)

    // TTF rlFont loading with custom generation parameters
    rlFont font = rlLoadFontEx("resources/KAISG.ttf", 96, 0, 0);

    // Generate mipmap levels to use trilinear filtering
    // NOTE: On 2D drawing it won't be noticeable, it looks like FILTER_BILINEAR
    GenTextureMipmaps(&font.texture);

    float fontSize = (float)font.baseSize;
    rlVector2 fontPosition = { 40.0f, screenHeight/2.0f - 80.0f };
    rlVector2 textSize = { 0.0f, 0.0f };

    // Setup texture scaling filter
    rlSetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
    int currentFontFilter = 0;      // TEXTURE_FILTER_POINT

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        fontSize += rlGetMouseWheelMove()*4.0f;

        // Choose font texture filter method
        if (rlIsKeyPressed(KEY_ONE))
        {
            rlSetTextureFilter(font.texture, TEXTURE_FILTER_POINT);
            currentFontFilter = 0;
        }
        else if (rlIsKeyPressed(KEY_TWO))
        {
            rlSetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
            currentFontFilter = 1;
        }
        else if (rlIsKeyPressed(KEY_THREE))
        {
            // NOTE: Trilinear filter won't be noticed on 2D drawing
            rlSetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);
            currentFontFilter = 2;
        }

        textSize = rlMeasureTextEx(font, msg, fontSize, 0);

        if (rlIsKeyDown(KEY_LEFT)) fontPosition.x -= 10;
        else if (rlIsKeyDown(KEY_RIGHT)) fontPosition.x += 10;

        // Load a dropped TTF file dynamically (at current fontSize)
        if (rlIsFileDropped())
        {
            rlFilePathList droppedFiles = rlLoadDroppedFiles();

            // NOTE: We only support first ttf file dropped
            if (rlIsFileExtension(droppedFiles.paths[0], ".ttf"))
            {
                rlUnloadFont(font);
                font = rlLoadFontEx(droppedFiles.paths[0], (int)fontSize, 0, 0);
            }
            
            rlUnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("Use mouse wheel to change font size", 20, 20, 10, GRAY);
            rlDrawText("Use KEY_RIGHT and KEY_LEFT to move text", 20, 40, 10, GRAY);
            rlDrawText("Use 1, 2, 3 to change texture filter", 20, 60, 10, GRAY);
            rlDrawText("Drop a new TTF font for dynamic loading", 20, 80, 10, DARKGRAY);

            rlDrawTextEx(font, msg, fontPosition, fontSize, 0, BLACK);

            // TODO: It seems texSize measurement is not accurate due to chars offsets...
            //rlDrawRectangleLines(fontPosition.x, fontPosition.y, textSize.x, textSize.y, RED);

            rlDrawRectangle(0, screenHeight - 80, screenWidth, 80, LIGHTGRAY);
            rlDrawText(rlTextFormat("rlFont size: %02.02f", fontSize), 20, screenHeight - 50, 10, DARKGRAY);
            rlDrawText(rlTextFormat("Text size: [%02.02f, %02.02f]", textSize.x, textSize.y), 20, screenHeight - 30, 10, DARKGRAY);
            rlDrawText("CURRENT TEXTURE FILTER:", 250, 400, 20, GRAY);

            if (currentFontFilter == 0) rlDrawText("POINT", 570, 400, 20, BLACK);
            else if (currentFontFilter == 1) rlDrawText("BILINEAR", 570, 400, 20, BLACK);
            else if (currentFontFilter == 2) rlDrawText("TRILINEAR", 570, 400, 20, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadFont(font);           // rlFont unloading

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}