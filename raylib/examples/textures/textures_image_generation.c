/*******************************************************************************************
*
*   raylib [textures] example - Procedural images generation
*
*   Example originally created with raylib 1.8, last time updated with raylib 1.8
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2O17-2024 Wilhem Barbier (@nounoursheureux) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define NUM_TEXTURES  9      // Currently we have 8 generation algorithms but some have multiple purposes (Linear and Square Gradients)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - procedural images generation");

    rlImage verticalGradient = rlGenImageGradientLinear(screenWidth, screenHeight, 0, RED, BLUE);
    rlImage horizontalGradient = rlGenImageGradientLinear(screenWidth, screenHeight, 90, RED, BLUE);
    rlImage diagonalGradient = rlGenImageGradientLinear(screenWidth, screenHeight, 45, RED, BLUE);
    rlImage radialGradient = rlGenImageGradientRadial(screenWidth, screenHeight, 0.0f, WHITE, BLACK);
    rlImage squareGradient = rlGenImageGradientSquare(screenWidth, screenHeight, 0.0f, WHITE, BLACK);
    rlImage checked = rlGenImageChecked(screenWidth, screenHeight, 32, 32, RED, BLUE);
    rlImage whiteNoise = rlGenImageWhiteNoise(screenWidth, screenHeight, 0.5f);
    rlImage perlinNoise = rlGenImagePerlinNoise(screenWidth, screenHeight, 50, 50, 4.0f);
    rlImage cellular = rlGenImageCellular(screenWidth, screenHeight, 32);

    Texture2D textures[NUM_TEXTURES] = { 0 };

    textures[0] = rlLoadTextureFromImage(verticalGradient);
    textures[1] = rlLoadTextureFromImage(horizontalGradient);
    textures[2] = rlLoadTextureFromImage(diagonalGradient);
    textures[3] = rlLoadTextureFromImage(radialGradient);
    textures[4] = rlLoadTextureFromImage(squareGradient);
    textures[5] = rlLoadTextureFromImage(checked);
    textures[6] = rlLoadTextureFromImage(whiteNoise);
    textures[7] = rlLoadTextureFromImage(perlinNoise);
    textures[8] = rlLoadTextureFromImage(cellular);

    // Unload image data (CPU RAM)
    rlUnloadImage(verticalGradient);
    rlUnloadImage(horizontalGradient);
    rlUnloadImage(diagonalGradient);
    rlUnloadImage(radialGradient);
    rlUnloadImage(squareGradient);
    rlUnloadImage(checked);
    rlUnloadImage(whiteNoise);
    rlUnloadImage(perlinNoise);
    rlUnloadImage(cellular);

    int currentTexture = 0;

    rlSetTargetFPS(60);
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())
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

            rlDrawTexture(textures[currentTexture], 0, 0, WHITE);

            rlDrawRectangle(30, 400, 325, 30, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines(30, 400, 325, 30, rlFade(WHITE, 0.5f));
            rlDrawText("MOUSE LEFT BUTTON to CYCLE PROCEDURAL TEXTURES", 40, 410, 10, WHITE);

            switch(currentTexture)
            {
                case 0: rlDrawText("VERTICAL GRADIENT", 560, 10, 20, RAYWHITE); break;
                case 1: rlDrawText("HORIZONTAL GRADIENT", 540, 10, 20, RAYWHITE); break;
                case 2: rlDrawText("DIAGONAL GRADIENT", 540, 10, 20, RAYWHITE); break;
                case 3: rlDrawText("RADIAL GRADIENT", 580, 10, 20, LIGHTGRAY); break;
                case 4: rlDrawText("SQUARE GRADIENT", 580, 10, 20, LIGHTGRAY); break;
                case 5: rlDrawText("CHECKED", 680, 10, 20, RAYWHITE); break;
                case 6: rlDrawText("WHITE NOISE", 640, 10, 20, RED); break;
                case 7: rlDrawText("PERLIN NOISE", 640, 10, 20, RED); break;
                case 8: rlDrawText("CELLULAR", 670, 10, 20, RAYWHITE); break;
                default: break;
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // Unload textures data (GPU VRAM)
    for (int i = 0; i < NUM_TEXTURES; i++) UnloadTexture(textures[i]);

    rlCloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
