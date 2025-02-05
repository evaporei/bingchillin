/*******************************************************************************************
*
*   raylib [audio] example - rlMusic playing (streaming)
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [audio] example - music playing (streaming)");

    rlInitAudioDevice();              // Initialize audio device

    rlMusic music = rlLoadMusicStream("resources/country.mp3");

    rlPlayMusicStream(music);

    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;             // rlMusic playing paused

    rlSetTargetFPS(30);               // Set our game to run at 30 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateMusicStream(music);   // Update music buffer with new stream data
        
        // Restart music playing (stop and play)
        if (rlIsKeyPressed(KEY_SPACE))
        {
            rlStopMusicStream(music);
            rlPlayMusicStream(music);
        }

        // Pause/Resume music playing
        if (rlIsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) rlPauseMusicStream(music);
            else rlResumeMusicStream(music);
        }

        // Get normalized time played for current music stream
        timePlayed = rlGetMusicTimePlayed(music)/rlGetMusicTimeLength(music);

        if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("MUSIC SHOULD BE PLAYING!", 255, 150, 20, LIGHTGRAY);

            rlDrawRectangle(200, 200, 400, 12, LIGHTGRAY);
            rlDrawRectangle(200, 200, (int)(timePlayed*400.0f), 12, MAROON);
            rlDrawRectangleLines(200, 200, 400, 12, GRAY);

            rlDrawText("PRESS SPACE TO RESTART MUSIC", 215, 250, 20, LIGHTGRAY);
            rlDrawText("PRESS P TO PAUSE/RESUME MUSIC", 208, 280, 20, LIGHTGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadMusicStream(music);   // Unload music stream buffers from RAM

    rlCloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
