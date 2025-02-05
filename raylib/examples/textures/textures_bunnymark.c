/*******************************************************************************************
*
*   raylib [textures] example - Bunnymark
*
*   Example originally created with raylib 1.6, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>                 // Required for: malloc(), free()

#define MAX_BUNNIES        50000    // 50K bunnies limit

// This is the maximum amount of elements (quads) per batch
// NOTE: This value is defined in [rlgl] module and can be changed there
#define MAX_BATCH_ELEMENTS  8192

typedef struct Bunny {
    rlVector2 position;
    rlVector2 speed;
    rlColor color;
} Bunny;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - bunnymark");

    // Load bunny texture
    Texture2D texBunny = LoadTexture("resources/wabbit_alpha.png");

    Bunny *bunnies = (Bunny *)malloc(MAX_BUNNIES*sizeof(Bunny));    // Bunnies array

    int bunniesCount = 0;           // Bunnies counter

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            // Create more bunnies
            for (int i = 0; i < 100; i++)
            {
                if (bunniesCount < MAX_BUNNIES)
                {
                    bunnies[bunniesCount].position = rlGetMousePosition();
                    bunnies[bunniesCount].speed.x = (float)rlGetRandomValue(-250, 250)/60.0f;
                    bunnies[bunniesCount].speed.y = (float)rlGetRandomValue(-250, 250)/60.0f;
                    bunnies[bunniesCount].color = (rlColor){ rlGetRandomValue(50, 240),
                                                       rlGetRandomValue(80, 240),
                                                       rlGetRandomValue(100, 240), 255 };
                    bunniesCount++;
                }
            }
        }

        // Update bunnies
        for (int i = 0; i < bunniesCount; i++)
        {
            bunnies[i].position.x += bunnies[i].speed.x;
            bunnies[i].position.y += bunnies[i].speed.y;

            if (((bunnies[i].position.x + texBunny.width/2) > rlGetScreenWidth()) ||
                ((bunnies[i].position.x + texBunny.width/2) < 0)) bunnies[i].speed.x *= -1;
            if (((bunnies[i].position.y + texBunny.height/2) > rlGetScreenHeight()) ||
                ((bunnies[i].position.y + texBunny.height/2 - 40) < 0)) bunnies[i].speed.y *= -1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            for (int i = 0; i < bunniesCount; i++)
            {
                // NOTE: When internal batch buffer limit is reached (MAX_BATCH_ELEMENTS),
                // a draw call is launched and buffer starts being filled again;
                // before issuing a draw call, updated vertex data from internal CPU buffer is send to GPU...
                // Process of sending data is costly and it could happen that GPU data has not been completely
                // processed for drawing while new data is tried to be sent (updating current in-use buffers)
                // it could generates a stall and consequently a frame drop, limiting the number of drawn bunnies
                rlDrawTexture(texBunny, (int)bunnies[i].position.x, (int)bunnies[i].position.y, bunnies[i].color);
            }

            rlDrawRectangle(0, 0, screenWidth, 40, BLACK);
            rlDrawText(rlTextFormat("bunnies: %i", bunniesCount), 120, 10, 20, GREEN);
            rlDrawText(rlTextFormat("batched draw calls: %i", 1 + bunniesCount/MAX_BATCH_ELEMENTS), 320, 10, 20, MAROON);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(bunnies);              // Unload bunnies data array

    UnloadTexture(texBunny);    // Unload bunny texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
