/*******************************************************************************************
*
*   raylib [audio] example - Playing sound multiple times
*
*   Example originally created with raylib 4.6
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Jeffery Myers (@JeffM2501)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_SOUNDS 10
rlSound soundArray[MAX_SOUNDS] = { 0 };
int currentSound;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [audio] example - playing sound multiple times");

    rlInitAudioDevice();      // Initialize audio device

    // load the sound list
    soundArray[0] = rlLoadSound("resources/sound.wav");         // Load WAV audio file into the first slot as the 'source' sound
                                                              // this sound owns the sample data
    for (int i = 1; i < MAX_SOUNDS; i++)
    {
        soundArray[i] = rlLoadSoundAlias(soundArray[0]);        // Load an alias of the sound into slots 1-9. These do not own the sound data, but can be played
    }
    currentSound = 0;                                         // set the sound list to the start

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyPressed(KEY_SPACE))
        {
            rlPlaySound(soundArray[currentSound]);            // play the next open sound slot
            currentSound++;                                 // increment the sound slot
            if (currentSound >= MAX_SOUNDS)                 // if the sound slot is out of bounds, go back to 0
                currentSound = 0;

            // Note: a better way would be to look at the list for the first sound that is not playing and use that slot
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("Press SPACE to PLAY a WAV sound!", 200, 180, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 1; i < MAX_SOUNDS; i++)
        rlUnloadSoundAlias(soundArray[i]);     // Unload sound aliases
    rlUnloadSound(soundArray[0]);              // Unload source sound data

    rlCloseAudioDevice();     // Close audio device

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}