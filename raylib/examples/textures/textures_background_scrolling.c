/*******************************************************************************************
*
*   raylib [textures] example - Background scrolling
*
*   Example originally created with raylib 2.0, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - background scrolling");

    // NOTE: Be careful, background width must be equal or bigger than screen width
    // if not, texture should be draw more than two times for scrolling effect
    Texture2D background = LoadTexture("resources/cyberpunk_street_background.png");
    Texture2D midground = LoadTexture("resources/cyberpunk_street_midground.png");
    Texture2D foreground = LoadTexture("resources/cyberpunk_street_foreground.png");

    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        // NOTE: rlTexture is scaled twice its size, so it sould be considered on scrolling
        if (scrollingBack <= -background.width*2) scrollingBack = 0;
        if (scrollingMid <= -midground.width*2) scrollingMid = 0;
        if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(rlGetColor(0x052c46ff));

            // Draw background image twice
            // NOTE: rlTexture is scaled twice its size
            rlDrawTextureEx(background, (rlVector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
            rlDrawTextureEx(background, (rlVector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

            // Draw midground image twice
            rlDrawTextureEx(midground, (rlVector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
            rlDrawTextureEx(midground, (rlVector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

            // Draw foreground image twice
            rlDrawTextureEx(foreground, (rlVector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
            rlDrawTextureEx(foreground, (rlVector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

            rlDrawText("BACKGROUND SCROLLING & PARALLAX", 10, 10, 20, RED);
            rlDrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", screenWidth - 330, screenHeight - 20, 10, RAYWHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(background);  // Unload background texture
    UnloadTexture(midground);   // Unload midground texture
    UnloadTexture(foreground);  // Unload foreground texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}