/*******************************************************************************************
*
*   raylib [core] example - Gamepad input
*
*   NOTE: This example requires a Gamepad connected to the system
*         raylib is configured to work with the following gamepads:
*                - Xbox 360 Controller (Xbox 360, Xbox One)
*                - PLAYSTATION(R)3 Controller
*         Check raylib.h for buttons configuration
*
*   Example originally created with raylib 1.1, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#define XBOX360_NAME_ID     "Xbox 360 Controller"
#define PS3_NAME_ID         "Sony PLAYSTATION(R)3 Controller"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - gamepad input");

    Texture2D texPs3Pad = LoadTexture("resources/ps3.png");
    Texture2D texXboxPad = LoadTexture("resources/xbox.png");

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int gamepad = 0; // which gamepad to display

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // ...
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            if (rlIsKeyPressed(KEY_LEFT) && gamepad > 0) gamepad--;
            if (rlIsKeyPressed(KEY_RIGHT)) gamepad++;

            if (rlIsGamepadAvailable(gamepad))
            {
                rlDrawText(rlTextFormat("GP%d: %s", gamepad, rlGetGamepadName(gamepad)), 10, 10, 10, BLACK);

                if (rlTextIsEqual(rlGetGamepadName(gamepad), XBOX360_LEGACY_NAME_ID) || rlTextIsEqual(rlGetGamepadName(gamepad), XBOX360_NAME_ID))
                {
                    rlDrawTexture(texXboxPad, 0, 0, DARKGRAY);

                    // Draw buttons: xbox home
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) rlDrawCircle(394, 89, 19, RED);

                    // Draw buttons: basic
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) rlDrawCircle(436, 150, 9, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) rlDrawCircle(352, 150, 9, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) rlDrawCircle(501, 151, 15, BLUE);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) rlDrawCircle(536, 187, 15, LIME);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) rlDrawCircle(572, 151, 15, MAROON);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) rlDrawCircle(536, 115, 15, GOLD);

                    // Draw buttons: d-pad
                    rlDrawRectangle(317, 202, 19, 71, BLACK);
                    rlDrawRectangle(293, 228, 69, 19, BLACK);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) rlDrawRectangle(317, 202, 19, 26, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) rlDrawRectangle(317, 202 + 45, 19, 26, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) rlDrawRectangle(292, 228, 25, 19, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) rlDrawRectangle(292 + 44, 228, 26, 19, RED);

                    // Draw buttons: left-right back
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) rlDrawCircle(259, 61, 20, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) rlDrawCircle(536, 61, 20, RED);

                    // Draw axis: left joystick

                    rlColor leftGamepadColor = BLACK;
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) leftGamepadColor = RED;
                    rlDrawCircle(259, 152, 39, BLACK);
                    rlDrawCircle(259, 152, 34, LIGHTGRAY);
                    rlDrawCircle(259 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X)*20),
                               152 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y)*20), 25, leftGamepadColor);

                    // Draw axis: right joystick
                    rlColor rightGamepadColor = BLACK;
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) rightGamepadColor = RED;
                    rlDrawCircle(461, 237, 38, BLACK);
                    rlDrawCircle(461, 237, 33, LIGHTGRAY);
                    rlDrawCircle(461 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X)*20),
                               237 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y)*20), 25, rightGamepadColor);

                    // Draw axis: left-right triggers
                    rlDrawRectangle(170, 30, 15, 70, GRAY);
                    rlDrawRectangle(604, 30, 15, 70, GRAY);
                    rlDrawRectangle(170, 30, 15, (int)(((1 + rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER))/2)*70), RED);
                    rlDrawRectangle(604, 30, 15, (int)(((1 + rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER))/2)*70), RED);

                    //rlDrawText(rlTextFormat("Xbox axis LT: %02.02f", rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER)), 10, 40, 10, BLACK);
                    //rlDrawText(rlTextFormat("Xbox axis RT: %02.02f", rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER)), 10, 60, 10, BLACK);
                }
                else if (rlTextIsEqual(rlGetGamepadName(gamepad), PS3_NAME_ID))
                {
                    rlDrawTexture(texPs3Pad, 0, 0, DARKGRAY);

                    // Draw buttons: ps
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) rlDrawCircle(396, 222, 13, RED);

                    // Draw buttons: basic
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) rlDrawRectangle(328, 170, 32, 13, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) rlDrawTriangle((rlVector2){ 436, 168 }, (rlVector2){ 436, 185 }, (rlVector2){ 464, 177 }, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) rlDrawCircle(557, 144, 13, LIME);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) rlDrawCircle(586, 173, 13, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) rlDrawCircle(557, 203, 13, VIOLET);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) rlDrawCircle(527, 173, 13, PINK);

                    // Draw buttons: d-pad
                    rlDrawRectangle(225, 132, 24, 84, BLACK);
                    rlDrawRectangle(195, 161, 84, 25, BLACK);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) rlDrawRectangle(225, 132, 24, 29, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) rlDrawRectangle(225, 132 + 54, 24, 30, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) rlDrawRectangle(195, 161, 30, 25, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) rlDrawRectangle(195 + 54, 161, 30, 25, RED);

                    // Draw buttons: left-right back buttons
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) rlDrawCircle(239, 82, 20, RED);
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) rlDrawCircle(557, 82, 20, RED);

                    // Draw axis: left joystick
                    rlColor leftGamepadColor = BLACK;
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) leftGamepadColor = RED;
                    rlDrawCircle(319, 255, 35, leftGamepadColor);
                    rlDrawCircle(319, 255, 31, LIGHTGRAY);
                    rlDrawCircle(319 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X) * 20),
                               255 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y) * 20), 25, leftGamepadColor);

                    // Draw axis: right joystick
                    rlColor rightGamepadColor = BLACK;
                    if (rlIsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) rightGamepadColor = RED;
                    rlDrawCircle(475, 255, 35, BLACK);
                    rlDrawCircle(475, 255, 31, LIGHTGRAY);
                    rlDrawCircle(475 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X) * 20),
                               255 + (int)(rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y) * 20), 25, rightGamepadColor);

                    // Draw axis: left-right triggers
                    rlDrawRectangle(169, 48, 15, 70, GRAY);
                    rlDrawRectangle(611, 48, 15, 70, GRAY);
                    rlDrawRectangle(169, 48, 15, (int)(((1 - rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER)) / 2) * 70), RED);
                    rlDrawRectangle(611, 48, 15, (int)(((1 - rlGetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER)) / 2) * 70), RED);
                }
                else
                {
                    rlDrawText("- GENERIC GAMEPAD -", 280, 180, 20, GRAY);

                    // TODO: Draw generic gamepad
                }

                rlDrawText(rlTextFormat("DETECTED AXIS [%i]:", rlGetGamepadAxisCount(0)), 10, 50, 10, MAROON);

                for (int i = 0; i < rlGetGamepadAxisCount(0); i++)
                {
                    rlDrawText(rlTextFormat("AXIS %i: %.02f", i, rlGetGamepadAxisMovement(0, i)), 20, 70 + 20*i, 10, DARKGRAY);
                }

                if (rlGetGamepadButtonPressed() != GAMEPAD_BUTTON_UNKNOWN) rlDrawText(rlTextFormat("DETECTED BUTTON: %i", rlGetGamepadButtonPressed()), 10, 430, 10, RED);
                else rlDrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);
            }
            else
            {
                rlDrawText(rlTextFormat("GP%d: NOT DETECTED", gamepad), 10, 10, 10, GRAY);

                rlDrawTexture(texXboxPad, 0, 0, LIGHTGRAY);
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texPs3Pad);
    UnloadTexture(texXboxPad);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
