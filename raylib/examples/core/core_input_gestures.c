/*******************************************************************************************
*
*   raylib [core] example - Input Gestures Detection
*
*   Example originally created with raylib 1.4, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_GESTURE_STRINGS   20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - input gestures");

    rlVector2 touchPosition = { 0, 0 };
    rlRectangle touchArea = { 220, 10, screenWidth - 230.0f, screenHeight - 20.0f };

    int gesturesCount = 0;
    char gestureStrings[MAX_GESTURE_STRINGS][32];

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    //rlSetGesturesEnabled(0b0000000000001001);   // Enable only some gestures to be detected

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        lastGesture = currentGesture;
        currentGesture = rlGetGestureDetected();
        touchPosition = rlGetTouchPosition(0);

        if (rlCheckCollisionPointRec(touchPosition, touchArea) && (currentGesture != GESTURE_NONE))
        {
            if (currentGesture != lastGesture)
            {
                // Store gesture string
                switch (currentGesture)
                {
                    case GESTURE_TAP: rlTextCopy(gestureStrings[gesturesCount], "GESTURE TAP"); break;
                    case GESTURE_DOUBLETAP: rlTextCopy(gestureStrings[gesturesCount], "GESTURE DOUBLETAP"); break;
                    case GESTURE_HOLD: rlTextCopy(gestureStrings[gesturesCount], "GESTURE HOLD"); break;
                    case GESTURE_DRAG: rlTextCopy(gestureStrings[gesturesCount], "GESTURE DRAG"); break;
                    case GESTURE_SWIPE_RIGHT: rlTextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE RIGHT"); break;
                    case GESTURE_SWIPE_LEFT: rlTextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE LEFT"); break;
                    case GESTURE_SWIPE_UP: rlTextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE UP"); break;
                    case GESTURE_SWIPE_DOWN: rlTextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE DOWN"); break;
                    case GESTURE_PINCH_IN: rlTextCopy(gestureStrings[gesturesCount], "GESTURE PINCH IN"); break;
                    case GESTURE_PINCH_OUT: rlTextCopy(gestureStrings[gesturesCount], "GESTURE PINCH OUT"); break;
                    default: break;
                }

                gesturesCount++;

                // Reset gestures strings
                if (gesturesCount >= MAX_GESTURE_STRINGS)
                {
                    for (int i = 0; i < MAX_GESTURE_STRINGS; i++) rlTextCopy(gestureStrings[i], "\0");

                    gesturesCount = 0;
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlDrawRectangleRec(touchArea, GRAY);
            rlDrawRectangle(225, 15, screenWidth - 240, screenHeight - 30, RAYWHITE);

            rlDrawText("GESTURES TEST AREA", screenWidth - 270, screenHeight - 40, 20, rlFade(GRAY, 0.5f));

            for (int i = 0; i < gesturesCount; i++)
            {
                if (i%2 == 0) rlDrawRectangle(10, 30 + 20*i, 200, 20, rlFade(LIGHTGRAY, 0.5f));
                else rlDrawRectangle(10, 30 + 20*i, 200, 20, rlFade(LIGHTGRAY, 0.3f));

                if (i < gesturesCount - 1) rlDrawText(gestureStrings[i], 35, 36 + 20*i, 10, DARKGRAY);
                else rlDrawText(gestureStrings[i], 35, 36 + 20*i, 10, MAROON);
            }

            rlDrawRectangleLines(10, 29, 200, screenHeight - 50, GRAY);
            rlDrawText("DETECTED GESTURES", 50, 15, 10, GRAY);

            if (currentGesture != GESTURE_NONE) rlDrawCircleV(touchPosition, 30, MAROON);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}