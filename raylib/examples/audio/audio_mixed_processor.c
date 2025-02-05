/*******************************************************************************************
*
*   raylib [audio] example - Mixed audio processing
*
*   Example originally created with raylib 4.2, last time updated with raylib 4.2
*
*   Example contributed by hkc (@hatkidchan) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 hkc (@hatkidchan)
*
********************************************************************************************/
#include "raylib.h"
#include <math.h>

static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[400] = { 0.0f };   // Average volume history

//------------------------------------------------------------------------------------
// Audio processing function
//------------------------------------------------------------------------------------
void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s
    float average = 0.0f;               // Temporary average volume

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];

        *left = powf(fabsf(*left), exponent) * ( (*left < 0.0f)? -1.0f : 1.0f );
        *right = powf(fabsf(*right), exponent) * ( (*right < 0.0f)? -1.0f : 1.0f );

        average += fabsf(*left) / frames;   // accumulating average volume
        average += fabsf(*right) / frames;
    }

    // Moving history to the left
    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];

    averageVolume[399] = average;         // Adding last average value
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [audio] example - processing mixed output");

    rlInitAudioDevice();              // Initialize audio device

    rlAttachAudioMixedProcessor(ProcessAudio);

    rlMusic music = rlLoadMusicStream("resources/country.mp3");
    rlSound sound = rlLoadSound("resources/coin.wav");

    rlPlayMusicStream(music);

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateMusicStream(music);   // Update music buffer with new stream data

        // Modify processing variables
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_LEFT)) exponent -= 0.05f;
        if (rlIsKeyPressed(KEY_RIGHT)) exponent += 0.05f;

        if (exponent <= 0.5f) exponent = 0.5f;
        if (exponent >= 3.0f) exponent = 3.0f;

        if (rlIsKeyPressed(KEY_SPACE)) rlPlaySound(sound);

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("MUSIC SHOULD BE PLAYING!", 255, 150, 20, LIGHTGRAY);

            rlDrawText(rlTextFormat("EXPONENT = %.2f", exponent), 215, 180, 20, LIGHTGRAY);

            rlDrawRectangle(199, 199, 402, 34, LIGHTGRAY);
            for (int i = 0; i < 400; i++)
            {
                rlDrawLine(201 + i, 232 - (int)(averageVolume[i] * 32), 201 + i, 232, MAROON);
            }
            rlDrawRectangleLines(199, 199, 402, 34, GRAY);

            rlDrawText("PRESS SPACE TO PLAY OTHER SOUND", 200, 250, 20, LIGHTGRAY);
            rlDrawText("USE LEFT AND RIGHT ARROWS TO ALTER DISTORTION", 140, 280, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadMusicStream(music);   // Unload music stream buffers from RAM

    rlDetachAudioMixedProcessor(ProcessAudio);  // Disconnect audio processor

    rlCloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
