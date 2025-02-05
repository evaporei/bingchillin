/*******************************************************************************************
*
*   raylib [others] example - Embedded files loading (rlWave and rlImage)
*
*   Example originally created with raylib 3.0, last time updated with raylib 2.5
*
*   Example contributed by Kristian Holmgren (@defutura) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2024 Kristian Holmgren (@defutura) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "resources/audio_data.h"   // rlWave file exported with rlExportWaveAsCode()
#include "resources/image_data.h"   // rlImage file exported with rlExportImageAsCode()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [others] example - embedded files loading");

    rlInitAudioDevice();              // Initialize audio device

    // Loaded in CPU memory (RAM) from header file (audio_data.h)
    // Same as: rlWave wave = rlLoadWave("sound.wav");
    rlWave wave = {
        .data = AUDIO_DATA,
        .frameCount = AUDIO_FRAME_COUNT,
        .sampleRate = AUDIO_SAMPLE_RATE,
        .sampleSize = AUDIO_SAMPLE_SIZE,
        .channels = AUDIO_CHANNELS
    };

    // rlWave converted to rlSound to be played
    rlSound sound = rlLoadSoundFromWave(wave);

    // With a rlWave loaded from file, after rlSound is loaded, we can unload rlWave
    // but in our case, rlWave is embedded in executable, in program .data segment
    // we can not (and should not) try to free that private memory region
    //rlUnloadWave(wave);             // Do not unload wave data!

    // Loaded in CPU memory (RAM) from header file (image_data.h)
    // Same as: rlImage image = rlLoadImage("raylib_logo.png");
    rlImage image = {
        .data = IMAGE_DATA,
        .width = IMAGE_WIDTH,
        .height = IMAGE_HEIGHT,
        .format = IMAGE_FORMAT,
        .mipmaps = 1
    };

    // rlImage converted to rlTexture (VRAM) to be drawn
    Texture2D texture = rlLoadTextureFromImage(image);

    // With an rlImage loaded from file, after rlTexture is loaded, we can unload rlImage
    // but in our case, rlImage is embedded in executable, in program .data segment
    // we can not (and should not) try to free that private memory region
    //rlUnloadImage(image);           // Do not unload image data!

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE)) rlPlaySound(sound);      // Play sound
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawTexture(texture, screenWidth/2 - texture.width/2, 40, WHITE);

            rlDrawText("raylib logo and sound loaded from header files", 150, 320, 20, LIGHTGRAY);
            rlDrawText("Press SPACE to PLAY the sound!", 220, 370, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadSound(sound);     // Unload sound from VRAM
    UnloadTexture(texture); // Unload texture from VRAM

    rlCloseAudioDevice();     // Close audio device

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}