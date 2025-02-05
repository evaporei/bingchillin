/*******************************************************************************************
*
*   raylib [core] example - 2D Camera system
*
*   Example originally created with raylib 1.5, last time updated with raylib 3.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_BUILDINGS   100

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    rlRectangle player = { 400, 280, 40, 40 };
    rlRectangle buildings[MAX_BUILDINGS] = { 0 };
    rlColor buildColors[MAX_BUILDINGS] = { 0 };

    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].width = (float)rlGetRandomValue(50, 200);
        buildings[i].height = (float)rlGetRandomValue(100, 800);
        buildings[i].y = screenHeight - 130.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;

        spacing += (int)buildings[i].width;

        buildColors[i] = (rlColor){ rlGetRandomValue(200, 240), rlGetRandomValue(200, 240), rlGetRandomValue(200, 250), 255 };
    }

    rlCamera2D camera = { 0 };
    camera.target = (rlVector2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Player movement
        if (rlIsKeyDown(KEY_RIGHT)) player.x += 2;
        else if (rlIsKeyDown(KEY_LEFT)) player.x -= 2;

        // Camera target follows player
        camera.target = (rlVector2){ player.x + 20, player.y + 20 };

        // Camera rotation controls
        if (rlIsKeyDown(KEY_A)) camera.rotation--;
        else if (rlIsKeyDown(KEY_S)) camera.rotation++;

        // Limit camera rotation to 80 degrees (-40 to 40)
        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40;

        // Camera zoom controls
        camera.zoom += ((float)rlGetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (rlIsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode2D(camera);

                rlDrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);

                for (int i = 0; i < MAX_BUILDINGS; i++) rlDrawRectangleRec(buildings[i], buildColors[i]);

                rlDrawRectangleRec(player, RED);

                rlDrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
                rlDrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);

            rlEndMode2D();

            rlDrawText("SCREEN AREA", 640, 10, 20, RED);

            rlDrawRectangle(0, 0, screenWidth, 5, RED);
            rlDrawRectangle(0, 5, 5, screenHeight - 10, RED);
            rlDrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
            rlDrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);

            rlDrawRectangle( 10, 10, 250, 113, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines( 10, 10, 250, 113, BLUE);

            rlDrawText("Free 2d camera controls:", 20, 20, 10, BLACK);
            rlDrawText("- Right/Left to move Offset", 40, 40, 10, DARKGRAY);
            rlDrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
            rlDrawText("- A / S to Rotate", 40, 80, 10, DARKGRAY);
            rlDrawText("- R to reset Zoom and Rotation", 40, 100, 10, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
