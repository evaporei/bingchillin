/*******************************************************************************************
*
*   raylib [audio] example - Module playing (streaming)
*
*   Example originally created with raylib 1.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_CIRCLES  64

typedef struct {
    rlVector2 position;
    float radius;
    float alpha;
    float speed;
    rlColor color;
} CircleWave;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X

    rlInitWindow(screenWidth, screenHeight, "raylib [audio] example - module playing (streaming)");

    rlInitAudioDevice();                  // Initialize audio device

    rlColor colors[14] = { ORANGE, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK,
                         YELLOW, GREEN, SKYBLUE, PURPLE, BEIGE };

    // Creates some circles for visual effect
    CircleWave circles[MAX_CIRCLES] = { 0 };

    for (int i = MAX_CIRCLES - 1; i >= 0; i--)
    {
        circles[i].alpha = 0.0f;
        circles[i].radius = (float)rlGetRandomValue(10, 40);
        circles[i].position.x = (float)rlGetRandomValue((int)circles[i].radius, (int)(screenWidth - circles[i].radius));
        circles[i].position.y = (float)rlGetRandomValue((int)circles[i].radius, (int)(screenHeight - circles[i].radius));
        circles[i].speed = (float)rlGetRandomValue(1, 100)/2000.0f;
        circles[i].color = colors[rlGetRandomValue(0, 13)];
    }

    rlMusic music = rlLoadMusicStream("resources/mini1111.xm");
    music.looping = false;
    float pitch = 1.0f;

    rlPlayMusicStream(music);

    float timePlayed = 0.0f;
    bool pause = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateMusicStream(music);      // Update music buffer with new stream data

        // Restart music playing (stop and play)
        if (rlIsKeyPressed(KEY_SPACE))
        {
            rlStopMusicStream(music);
            rlPlayMusicStream(music);
            pause = false;
        }

        // Pause/Resume music playing
        if (rlIsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) rlPauseMusicStream(music);
            else rlResumeMusicStream(music);
        }

        if (rlIsKeyDown(KEY_DOWN)) pitch -= 0.01f;
        else if (rlIsKeyDown(KEY_UP)) pitch += 0.01f;

        rlSetMusicPitch(music, pitch);

        // Get timePlayed scaled to bar dimensions
        timePlayed = rlGetMusicTimePlayed(music)/rlGetMusicTimeLength(music)*(screenWidth - 40);

        // rlColor circles animation
        for (int i = MAX_CIRCLES - 1; (i >= 0) && !pause; i--)
        {
            circles[i].alpha += circles[i].speed;
            circles[i].radius += circles[i].speed*10.0f;

            if (circles[i].alpha > 1.0f) circles[i].speed *= -1;

            if (circles[i].alpha <= 0.0f)
            {
                circles[i].alpha = 0.0f;
                circles[i].radius = (float)rlGetRandomValue(10, 40);
                circles[i].position.x = (float)rlGetRandomValue((int)circles[i].radius, (int)(screenWidth - circles[i].radius));
                circles[i].position.y = (float)rlGetRandomValue((int)circles[i].radius, (int)(screenHeight - circles[i].radius));
                circles[i].color = colors[rlGetRandomValue(0, 13)];
                circles[i].speed = (float)rlGetRandomValue(1, 100)/2000.0f;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            for (int i = MAX_CIRCLES - 1; i >= 0; i--)
            {
                rlDrawCircleV(circles[i].position, circles[i].radius, rlFade(circles[i].color, circles[i].alpha));
            }

            // Draw time bar
            rlDrawRectangle(20, screenHeight - 20 - 12, screenWidth - 40, 12, LIGHTGRAY);
            rlDrawRectangle(20, screenHeight - 20 - 12, (int)timePlayed, 12, MAROON);
            rlDrawRectangleLines(20, screenHeight - 20 - 12, screenWidth - 40, 12, GRAY);

            // Draw help instructions
            rlDrawRectangle(20, 20, 425, 145, WHITE);
            rlDrawRectangleLines(20, 20, 425, 145, GRAY);
            rlDrawText("PRESS SPACE TO RESTART MUSIC", 40, 40, 20, BLACK);
            rlDrawText("PRESS P TO PAUSE/RESUME", 40, 70, 20, BLACK);
            rlDrawText("PRESS UP/DOWN TO CHANGE SPEED", 40, 100, 20, BLACK);
            rlDrawText(rlTextFormat("SPEED: %f", pitch), 40, 130, 20, MAROON);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadMusicStream(music);          // Unload music stream buffers from RAM

    rlCloseAudioDevice();     // Close audio device (music streaming is automatically stopped)

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
