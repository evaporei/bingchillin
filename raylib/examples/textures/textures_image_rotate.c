/*******************************************************************************************
*
*   raylib [textures] example - rlImage Rotation
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

#define NUM_TEXTURES  3

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - texture rotation");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    rlImage image45 = rlLoadImage("resources/raylib_logo.png");
    rlImage image90 = rlLoadImage("resources/raylib_logo.png");
    rlImage imageNeg90 = rlLoadImage("resources/raylib_logo.png");

    rlImageRotate(&image45, 45);
    rlImageRotate(&image90, 90);
    rlImageRotate(&imageNeg90, -90);

    Texture2D textures[NUM_TEXTURES] = { 0 };

    textures[0] = rlLoadTextureFromImage(image45);
    textures[1] = rlLoadTextureFromImage(image90);
    textures[2] = rlLoadTextureFromImage(imageNeg90);

    int currentTexture = 0;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT) || rlIsKeyPressed(KEY_RIGHT))
        {
            currentTexture = (currentTexture + 1)%NUM_TEXTURES; // Cycle between the textures
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawTexture(textures[currentTexture], screenWidth/2 - textures[currentTexture].width/2, screenHeight/2 - textures[currentTexture].height/2, WHITE);

            rlDrawText("Press LEFT MOUSE BUTTON to rotate the image clockwise", 250, 420, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < NUM_TEXTURES; i++) UnloadTexture(textures[i]);

    rlCloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
