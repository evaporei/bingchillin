/*******************************************************************************************
*
*   raylib [shapes] example - Draw basic shapes 2d (rectangle, circle, line...)
*
*   Example originally created with raylib 1.0, last time updated with raylib 4.2
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");

    float rotation = 0.0f;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rotation += 0.2f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY);

            // Circle shapes and lines
            rlDrawCircle(screenWidth/5, 120, 35, DARKBLUE);
            rlDrawCircleGradient(screenWidth/5, 220, 60, GREEN, SKYBLUE);
            rlDrawCircleLines(screenWidth/5, 340, 80, DARKBLUE);

            // rlRectangle shapes and lines
            rlDrawRectangle(screenWidth/4*2 - 60, 100, 120, 60, RED);
            rlDrawRectangleGradientH(screenWidth/4*2 - 90, 170, 180, 130, MAROON, GOLD);
            rlDrawRectangleLines(screenWidth/4*2 - 40, 320, 80, 60, ORANGE);  // NOTE: Uses QUADS internally, not lines

            // Triangle shapes and lines
            rlDrawTriangle((rlVector2){ screenWidth/4.0f *3.0f, 80.0f },
                         (rlVector2){ screenWidth/4.0f *3.0f - 60.0f, 150.0f },
                         (rlVector2){ screenWidth/4.0f *3.0f + 60.0f, 150.0f }, VIOLET);

            rlDrawTriangleLines((rlVector2){ screenWidth/4.0f*3.0f, 160.0f },
                              (rlVector2){ screenWidth/4.0f*3.0f - 20.0f, 230.0f },
                              (rlVector2){ screenWidth/4.0f*3.0f + 20.0f, 230.0f }, DARKBLUE);

            // Polygon shapes and lines
            rlDrawPoly((rlVector2){ screenWidth/4.0f*3, 330 }, 6, 80, rotation, BROWN);
            rlDrawPolyLines((rlVector2){ screenWidth/4.0f*3, 330 }, 6, 90, rotation, BROWN);
            rlDrawPolyLinesEx((rlVector2){ screenWidth/4.0f*3, 330 }, 6, 85, rotation, 6, BEIGE);

            // NOTE: We draw all LINES based shapes together to optimize internal drawing,
            // this way, all LINES are rendered in a single draw pass
            rlDrawLine(18, 42, screenWidth - 18, 42, BLACK);
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
