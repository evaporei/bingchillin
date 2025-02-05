/*******************************************************************************************
*
*   raylib example - particles blending
*
*   Example originally created with raylib 1.7, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_PARTICLES 200

// Particle structure with basic data
typedef struct {
    rlVector2 position;
    rlColor color;
    float alpha;
    float size;
    float rotation;
    bool active;        // NOTE: Use it to activate/deactive particle
} Particle;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [textures] example - particles blending");

    // Particles pool, reuse them!
    Particle mouseTail[MAX_PARTICLES] = { 0 };

    // Initialize particles
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        mouseTail[i].position = (rlVector2){ 0, 0 };
        mouseTail[i].color = (rlColor){ rlGetRandomValue(0, 255), rlGetRandomValue(0, 255), rlGetRandomValue(0, 255), 255 };
        mouseTail[i].alpha = 1.0f;
        mouseTail[i].size = (float)rlGetRandomValue(1, 30)/20.0f;
        mouseTail[i].rotation = (float)rlGetRandomValue(0, 360);
        mouseTail[i].active = false;
    }

    float gravity = 3.0f;

    Texture2D smoke = LoadTexture("resources/spark_flame.png");

    int blending = BLEND_ALPHA;

    rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Activate one particle every frame and Update active particles
        // NOTE: Particles initial position should be mouse position when activated
        // NOTE: Particles fall down with gravity and rotation... and disappear after 2 seconds (alpha = 0)
        // NOTE: When a particle disappears, active = false and it can be reused.
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (!mouseTail[i].active)
            {
                mouseTail[i].active = true;
                mouseTail[i].alpha = 1.0f;
                mouseTail[i].position = rlGetMousePosition();
                i = MAX_PARTICLES;
            }
        }

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (mouseTail[i].active)
            {
                mouseTail[i].position.y += gravity/2;
                mouseTail[i].alpha -= 0.005f;

                if (mouseTail[i].alpha <= 0.0f) mouseTail[i].active = false;

                mouseTail[i].rotation += 2.0f;
            }
        }

        if (rlIsKeyPressed(KEY_SPACE))
        {
            if (blending == BLEND_ALPHA) blending = BLEND_ADDITIVE;
            else blending = BLEND_ALPHA;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(DARKGRAY);

            rlBeginBlendMode(blending);

                // Draw active particles
                for (int i = 0; i < MAX_PARTICLES; i++)
                {
                    if (mouseTail[i].active) rlDrawTexturePro(smoke, (rlRectangle){ 0.0f, 0.0f, (float)smoke.width, (float)smoke.height },
                                                           (rlRectangle){ mouseTail[i].position.x, mouseTail[i].position.y, smoke.width*mouseTail[i].size, smoke.height*mouseTail[i].size },
                                                           (rlVector2){ (float)(smoke.width*mouseTail[i].size/2.0f), (float)(smoke.height*mouseTail[i].size/2.0f) }, mouseTail[i].rotation,
                                                           rlFade(mouseTail[i].color, mouseTail[i].alpha));
                }

            rlEndBlendMode();

            rlDrawText("PRESS SPACE to CHANGE BLENDING MODE", 180, 20, 20, BLACK);

            if (blending == BLEND_ALPHA) rlDrawText("ALPHA BLENDING", 290, screenHeight - 40, 20, BLACK);
            else rlDrawText("ADDITIVE BLENDING", 280, screenHeight - 40, 20, RAYWHITE);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(smoke);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}