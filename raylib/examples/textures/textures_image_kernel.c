/*******************************************************************************************
*
*   raylib [textures] example - rlImage loading and texture creation
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Karim Salem (@kimo-s)
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void NormalizeKernel(float *kernel, int size)
{
    float sum = 0.0f;
    for (int i = 0; i < size; i++) sum += kernel[i]; 

    if (sum != 0.0f)
    {
        for (int i = 0; i < size; i++) kernel[i] /= sum; 
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - image convolution");
        
    rlImage image = rlLoadImage("resources/cat.png");     // Loaded in CPU memory (RAM)

    float gaussiankernel[] = { 
        1.0f, 2.0f, 1.0f,
        2.0f, 4.0f, 2.0f,
        1.0f, 2.0f, 1.0f };

    float sobelkernel[] = {
        1.0f, 0.0f, -1.0f,
        2.0f, 0.0f, -2.0f,
        1.0f, 0.0f, -1.0f };

    float sharpenkernel[] = {
        0.0f, -1.0f, 0.0f,
       -1.0f, 5.0f, -1.0f,
        0.0f, -1.0f, 0.0f };

    NormalizeKernel(gaussiankernel, 9);
    NormalizeKernel(sharpenkernel, 9);
    NormalizeKernel(sobelkernel, 9);

    rlImage catSharpend = rlImageCopy(image);
    rlImageKernelConvolution(&catSharpend, sharpenkernel, 9);
 
    rlImage catSobel = rlImageCopy(image);
    rlImageKernelConvolution(&catSobel, sobelkernel, 9);

    rlImage catGaussian = rlImageCopy(image);
    
    for (int i = 0; i < 6; i++)
    {
        rlImageKernelConvolution(&catGaussian, gaussiankernel, 9);
    }

    rlImageCrop(&image, (rlRectangle){ 0, 0, (float)200, (float)450 });
    rlImageCrop(&catGaussian, (rlRectangle){ 0, 0, (float)200, (float)450 });
    rlImageCrop(&catSobel, (rlRectangle){ 0, 0, (float)200, (float)450 });
    rlImageCrop(&catSharpend, (rlRectangle){ 0, 0, (float)200, (float)450 });
    
    // Images converted to texture, GPU memory (VRAM)
    Texture2D texture = rlLoadTextureFromImage(image);
    Texture2D catSharpendTexture = rlLoadTextureFromImage(catSharpend);
    Texture2D catSobelTexture = rlLoadTextureFromImage(catSobel);
    Texture2D catGaussianTexture = rlLoadTextureFromImage(catGaussian);
    
    // Once images have been converted to texture and uploaded to VRAM, 
    // they can be unloaded from RAM
    rlUnloadImage(image);
    rlUnloadImage(catGaussian);
    rlUnloadImage(catSobel);
    rlUnloadImage(catSharpend);

    rlSetTargetFPS(60);     // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

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

            rlDrawTexture(catSharpendTexture, 0, 0, WHITE);
            rlDrawTexture(catSobelTexture, 200, 0, WHITE);
            rlDrawTexture(catGaussianTexture, 400, 0, WHITE);
            rlDrawTexture(texture, 600, 0, WHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);
    UnloadTexture(catGaussianTexture);
    UnloadTexture(catSobelTexture);
    UnloadTexture(catSharpendTexture);

    rlCloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
