/*******************************************************************************************
*
*   raylib [textures] example - Retrive image channel (mask)
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   Example originally created with raylib 5.1-dev, last time updated with raylib 5.1-dev
*
*   Example contributed by Bruno Cabral (github.com/brccabral) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2024-2024 Bruno Cabral (github.com/brccabral) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - extract channel from image");

    rlImage fudesumiImage = rlLoadImage("resources/fudesumi.png");

    rlImage imageAlpha = rlImageFromChannel(fudesumiImage, 3);
    rlImageAlphaMask(&imageAlpha, imageAlpha);

    rlImage imageRed = rlImageFromChannel(fudesumiImage, 0);
    rlImageAlphaMask(&imageRed, imageAlpha);

    rlImage imageGreen = rlImageFromChannel(fudesumiImage, 1);
    rlImageAlphaMask(&imageGreen, imageAlpha);

    rlImage imageBlue = rlImageFromChannel(fudesumiImage, 2);
    rlImageAlphaMask(&imageBlue, imageAlpha);

    rlImage backgroundImage = rlGenImageChecked(screenWidth, screenHeight, screenWidth/20, screenHeight/20, ORANGE, YELLOW);

    Texture2D fudesumiTexture = rlLoadTextureFromImage(fudesumiImage);
    Texture2D textureAlpha = rlLoadTextureFromImage(imageAlpha);
    Texture2D textureRed = rlLoadTextureFromImage(imageRed);
    Texture2D textureGreen = rlLoadTextureFromImage(imageGreen);
    Texture2D textureBlue = rlLoadTextureFromImage(imageBlue);
    Texture2D backgroundTexture = rlLoadTextureFromImage(backgroundImage);

    rlUnloadImage(fudesumiImage);
    rlUnloadImage(imageAlpha);
    rlUnloadImage(imageRed);
    rlUnloadImage(imageGreen);
    rlUnloadImage(imageBlue);
    rlUnloadImage(backgroundImage);

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    rlRectangle fudesumiRec = {0, 0, fudesumiImage.width, fudesumiImage.height};

    rlRectangle fudesumiPos = {50, 10, fudesumiImage.width*0.8f, fudesumiImage.height*0.8f};
    rlRectangle redPos = { 410, 10, fudesumiPos.width / 2, fudesumiPos.height / 2 };
    rlRectangle greenPos = { 600, 10, fudesumiPos.width / 2, fudesumiPos.height / 2 };
    rlRectangle bluePos = { 410, 230, fudesumiPos.width / 2, fudesumiPos.height / 2 };
    rlRectangle alphaPos = { 600, 230, fudesumiPos.width / 2, fudesumiPos.height / 2 };

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlDrawTexture(backgroundTexture, 0, 0, WHITE);
            rlDrawTexturePro(fudesumiTexture, fudesumiRec, fudesumiPos, (rlVector2) {0, 0}, 0, WHITE);

            rlDrawTexturePro(textureRed, fudesumiRec, redPos, (rlVector2) {0, 0}, 0, RED);
            rlDrawTexturePro(textureGreen, fudesumiRec, greenPos, (rlVector2) {0, 0}, 0, GREEN);
            rlDrawTexturePro(textureBlue, fudesumiRec, bluePos, (rlVector2) {0, 0}, 0, BLUE);
            rlDrawTexturePro(textureAlpha, fudesumiRec, alphaPos, (rlVector2) {0, 0}, 0, WHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(backgroundTexture);
    UnloadTexture(fudesumiTexture);
    UnloadTexture(textureRed);
    UnloadTexture(textureGreen);
    UnloadTexture(textureBlue);
    UnloadTexture(textureAlpha);
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
