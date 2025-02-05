/*******************************************************************************************
*
*   raylib [textures] example - rlImage processing
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   Example originally created with raylib 1.4, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>             // Required for: free()

#define NUM_PROCESSES    9

typedef enum {
    NONE = 0,
    COLOR_GRAYSCALE,
    COLOR_TINT,
    COLOR_INVERT,
    COLOR_CONTRAST,
    COLOR_BRIGHTNESS,
    GAUSSIAN_BLUR,
    FLIP_VERTICAL,
    FLIP_HORIZONTAL
} ImageProcess;

static const char *processText[] = {
    "NO PROCESSING",
    "COLOR GRAYSCALE",
    "COLOR TINT",
    "COLOR INVERT",
    "COLOR CONTRAST",
    "COLOR BRIGHTNESS",
    "GAUSSIAN BLUR",
    "FLIP VERTICAL",
    "FLIP HORIZONTAL"
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - image processing");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    rlImage imOrigin = rlLoadImage("resources/parrots.png");   // Loaded in CPU memory (RAM)
    rlImageFormat(&imOrigin, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);         // Format image to RGBA 32bit (required for texture update) <-- ISSUE
    Texture2D texture = rlLoadTextureFromImage(imOrigin);    // rlImage converted to texture, GPU memory (VRAM)

    rlImage imCopy = rlImageCopy(imOrigin);

    int currentProcess = NONE;
    bool textureReload = false;

    rlRectangle toggleRecs[NUM_PROCESSES] = { 0 };
    int mouseHoverRec = -1;

    for (int i = 0; i < NUM_PROCESSES; i++) toggleRecs[i] = (rlRectangle){ 40.0f, (float)(50 + 32*i), 150.0f, 30.0f };

    rlSetTargetFPS(60);
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Mouse toggle group logic
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (rlCheckCollisionPointRec(rlGetMousePosition(), toggleRecs[i]))
            {
                mouseHoverRec = i;

                if (rlIsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    currentProcess = i;
                    textureReload = true;
                }
                break;
            }
            else mouseHoverRec = -1;
        }

        // Keyboard toggle group logic
        if (rlIsKeyPressed(KEY_DOWN))
        {
            currentProcess++;
            if (currentProcess > (NUM_PROCESSES - 1)) currentProcess = 0;
            textureReload = true;
        }
        else if (rlIsKeyPressed(KEY_UP))
        {
            currentProcess--;
            if (currentProcess < 0) currentProcess = 7;
            textureReload = true;
        }

        // Reload texture when required
        if (textureReload)
        {
            rlUnloadImage(imCopy);                // Unload image-copy data
            imCopy = rlImageCopy(imOrigin);     // Restore image-copy from image-origin

            // NOTE: rlImage processing is a costly CPU process to be done every frame,
            // If image processing is required in a frame-basis, it should be done
            // with a texture and by shaders
            switch (currentProcess)
            {
                case COLOR_GRAYSCALE: rlImageColorGrayscale(&imCopy); break;
                case COLOR_TINT: rlImageColorTint(&imCopy, GREEN); break;
                case COLOR_INVERT: rlImageColorInvert(&imCopy); break;
                case COLOR_CONTRAST: rlImageColorContrast(&imCopy, -40); break;
                case COLOR_BRIGHTNESS: rlImageColorBrightness(&imCopy, -80); break;
                case GAUSSIAN_BLUR: rlImageBlurGaussian(&imCopy, 10); break;
                case FLIP_VERTICAL: rlImageFlipVertical(&imCopy); break;
                case FLIP_HORIZONTAL: rlImageFlipHorizontal(&imCopy); break;
                default: break;
            }

            rlColor *pixels = rlLoadImageColors(imCopy);    // Load pixel data from image (RGBA 32bit)
            UpdateTexture(texture, pixels);             // Update texture with new image data
            rlUnloadImageColors(pixels);                  // Unload pixels data from RAM

            textureReload = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("IMAGE PROCESSING:", 40, 30, 10, DARKGRAY);

            // Draw rectangles
            for (int i = 0; i < NUM_PROCESSES; i++)
            {
                rlDrawRectangleRec(toggleRecs[i], ((i == currentProcess) || (i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                rlDrawRectangleLines((int)toggleRecs[i].x, (int) toggleRecs[i].y, (int) toggleRecs[i].width, (int) toggleRecs[i].height, ((i == currentProcess) || (i == mouseHoverRec)) ? BLUE : GRAY);
                rlDrawText( processText[i], (int)( toggleRecs[i].x + toggleRecs[i].width/2 - rlMeasureText(processText[i], 10)/2), (int) toggleRecs[i].y + 11, 10, ((i == currentProcess) || (i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
            }

            rlDrawTexture(texture, screenWidth - texture.width - 60, screenHeight/2 - texture.height/2, WHITE);
            rlDrawRectangleLines(screenWidth - texture.width - 60, screenHeight/2 - texture.height/2, texture.width, texture.height, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);       // Unload texture from VRAM
    rlUnloadImage(imOrigin);        // Unload image-origin from RAM
    rlUnloadImage(imCopy);          // Unload image-copy from RAM

    rlCloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}