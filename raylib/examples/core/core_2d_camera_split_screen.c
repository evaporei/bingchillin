/*******************************************************************************************
*
*   raylib [core] example - 2d camera split screen
*
*   Addapted from the core_3d_camera_split_screen example: 
*       https://github.com/raysan5/raylib/blob/master/examples/core/core_3d_camera_split_screen.c
*
*   Example originally created with raylib 4.5, last time updated with raylib 4.5
*
*   Example contributed by Gabriel dos Santos Sanches (@gabrielssanches) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Gabriel dos Santos Sanches (@gabrielssanches)
*
********************************************************************************************/

#include "raylib.h"

#define PLAYER_SIZE 40

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 440;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera split screen");

    rlRectangle player1 = { 200, 200, PLAYER_SIZE, PLAYER_SIZE };
    rlRectangle player2 = { 250, 200, PLAYER_SIZE, PLAYER_SIZE };

    rlCamera2D camera1 = { 0 };
    camera1.target = (rlVector2){ player1.x, player1.y };
    camera1.offset = (rlVector2){ 200.0f, 200.0f };
    camera1.rotation = 0.0f;
    camera1.zoom = 1.0f;

    rlCamera2D camera2 = { 0 };
    camera2.target = (rlVector2){ player2.x, player2.y };
    camera2.offset = (rlVector2){ 200.0f, 200.0f };
    camera2.rotation = 0.0f;
    camera2.zoom = 1.0f;

    rlRenderTexture screenCamera1 = rlLoadRenderTexture(screenWidth/2, screenHeight);
    rlRenderTexture screenCamera2 = rlLoadRenderTexture(screenWidth/2, screenHeight);

    // Build a flipped rectangle the size of the split view to use for drawing later
    rlRectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_S)) player1.y += 3.0f;
        else if (rlIsKeyDown(KEY_W)) player1.y -= 3.0f;
        if (rlIsKeyDown(KEY_D)) player1.x += 3.0f;
        else if (rlIsKeyDown(KEY_A)) player1.x -= 3.0f;

        if (rlIsKeyDown(KEY_UP)) player2.y -= 3.0f;
        else if (rlIsKeyDown(KEY_DOWN)) player2.y += 3.0f;
        if (rlIsKeyDown(KEY_RIGHT)) player2.x += 3.0f;
        else if (rlIsKeyDown(KEY_LEFT)) player2.x -= 3.0f;

        camera1.target = (rlVector2){ player1.x, player1.y };
        camera2.target = (rlVector2){ player2.x, player2.y };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginTextureMode(screenCamera1);
            rlClearBackground(RAYWHITE);
            
            rlBeginMode2D(camera1);
            
                // Draw full scene with first camera
                for (int i = 0; i < screenWidth/PLAYER_SIZE + 1; i++)
                {
                    rlDrawLineV((rlVector2){(float)PLAYER_SIZE*i, 0}, (rlVector2){ (float)PLAYER_SIZE*i, (float)screenHeight}, LIGHTGRAY);
                }

                for (int i = 0; i < screenHeight/PLAYER_SIZE + 1; i++)
                {
                    rlDrawLineV((rlVector2){0, (float)PLAYER_SIZE*i}, (rlVector2){ (float)screenWidth, (float)PLAYER_SIZE*i}, LIGHTGRAY);
                }

                for (int i = 0; i < screenWidth/PLAYER_SIZE; i++)
                {
                    for (int j = 0; j < screenHeight/PLAYER_SIZE; j++)
                    {
                        rlDrawText(rlTextFormat("[%i,%i]", i, j), 10 + PLAYER_SIZE*i, 15 + PLAYER_SIZE*j, 10, LIGHTGRAY);
                    }
                }

                rlDrawRectangleRec(player1, RED);
                rlDrawRectangleRec(player2, BLUE);
            rlEndMode2D();
            
            rlDrawRectangle(0, 0, rlGetScreenWidth()/2, 30, rlFade(RAYWHITE, 0.6f));
            rlDrawText("PLAYER1: W/S/A/D to move", 10, 10, 10, MAROON);
            
        rlEndTextureMode();

        rlBeginTextureMode(screenCamera2);
            rlClearBackground(RAYWHITE);
            
            rlBeginMode2D(camera2);
            
                // Draw full scene with second camera
                for (int i = 0; i < screenWidth/PLAYER_SIZE + 1; i++)
                {
                    rlDrawLineV((rlVector2){ (float)PLAYER_SIZE*i, 0}, (rlVector2){ (float)PLAYER_SIZE*i, (float)screenHeight}, LIGHTGRAY);
                }

                for (int i = 0; i < screenHeight/PLAYER_SIZE + 1; i++)
                {
                    rlDrawLineV((rlVector2){0, (float)PLAYER_SIZE*i}, (rlVector2){ (float)screenWidth, (float)PLAYER_SIZE*i}, LIGHTGRAY);
                }

                for (int i = 0; i < screenWidth/PLAYER_SIZE; i++)
                {
                    for (int j = 0; j < screenHeight/PLAYER_SIZE; j++)
                    {
                        rlDrawText(rlTextFormat("[%i,%i]", i, j), 10 + PLAYER_SIZE*i, 15 + PLAYER_SIZE*j, 10, LIGHTGRAY);
                    }
                }

                rlDrawRectangleRec(player1, RED);
                rlDrawRectangleRec(player2, BLUE);
                
            rlEndMode2D();
            
            rlDrawRectangle(0, 0, rlGetScreenWidth()/2, 30, rlFade(RAYWHITE, 0.6f));
            rlDrawText("PLAYER2: UP/DOWN/LEFT/RIGHT to move", 10, 10, 10, DARKBLUE);
            
        rlEndTextureMode();

        // Draw both views render textures to the screen side by side
        rlBeginDrawing();
            rlClearBackground(BLACK);
            
            rlDrawTextureRec(screenCamera1.texture, splitScreenRect, (rlVector2){ 0, 0 }, WHITE);
            rlDrawTextureRec(screenCamera2.texture, splitScreenRect, (rlVector2){ screenWidth/2.0f, 0 }, WHITE);
            
            rlDrawRectangle(rlGetScreenWidth()/2 - 2, 0, 4, rlGetScreenHeight(), LIGHTGRAY);
        rlEndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadRenderTexture(screenCamera1); // Unload render texture
    rlUnloadRenderTexture(screenCamera2); // Unload render texture

    rlCloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
