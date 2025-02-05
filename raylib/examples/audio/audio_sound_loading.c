/*******************************************************************************************
*
*   raylib [audio] example - rlSound loading and playing
*
*   Example originally created with raylib 1.1, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [audio] example - sound loading and playing");

    rlInitAudioDevice();      // Initialize audio device

    rlSound fxWav = rlLoadSound("resources/sound.wav");         // Load WAV audio file
    rlSound fxOgg = rlLoadSound("resources/target.ogg");        // Load OGG audio file

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE)) rlPlaySound(fxWav);      // Play WAV sound
        if (rlIsKeyPressed(KEY_ENTER)) rlPlaySound(fxOgg);      // Play OGG sound
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("Press SPACE to PLAY the WAV sound!", 200, 180, 20, LIGHTGRAY);
            rlDrawText("Press ENTER to PLAY the OGG sound!", 200, 220, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadSound(fxWav);     // Unload sound data
    rlUnloadSound(fxOgg);     // Unload sound data

    rlCloseAudioDevice();     // Close audio device

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}