/*******************************************************************************************
*
*   raylib [core] example - custom frame control
*
*   NOTE: WARNING: This is an example for advanced users willing to have full control over
*   the frame processes. By default, rlEndDrawing() calls the following processes:
*       1. Draw remaining batch data: rlDrawRenderBatchActive()
*       2. rlSwapScreenBuffer()
*       3. Frame time control: rlWaitTime()
*       4. rlPollInputEvents()
*
*   To avoid steps 2, 3 and 4, flag SUPPORT_CUSTOM_FRAME_CONTROL can be enabled in
*   config.h (it requires recompiling raylib). This way those steps are up to the user.
*
*   Note that enabling this flag invalidates some functions:
*       - rlGetFrameTime()
*       - rlSetTargetFPS()
*       - rlGetFPS()
*
*   Example originally created with raylib 4.0, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Ramon Santamaria (@raysan5)
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
    
    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - custom frame control");

    // Custom timming variables
    double previousTime = rlGetTime();    // Previous time measure
    double currentTime = 0.0;           // Current time measure
    double updateDrawTime = 0.0;        // Update + Draw time
    double waitTime = 0.0;              // Wait time (if target fps required)
    float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)
    
    float timeCounter = 0.0f;           // Accumulative time counter (seconds)
    float position = 0.0f;              // Circle position
    bool pause = false;                 // Pause control flag
    
    int targetFPS = 60;                 // Our initial target fps
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlPollInputEvents();              // Poll input events (SUPPORT_CUSTOM_FRAME_CONTROL)
        
        if (rlIsKeyPressed(KEY_SPACE)) pause = !pause;
        
        if (rlIsKeyPressed(KEY_UP)) targetFPS += 20;
        else if (rlIsKeyPressed(KEY_DOWN)) targetFPS -= 20;
        
        if (targetFPS < 0) targetFPS = 0;

        if (!pause)
        {
            position += 200*deltaTime;  // We move at 200 pixels per second
            if (position >= rlGetScreenWidth()) position = 0;
            timeCounter += deltaTime;   // We count time (seconds)
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            for (int i = 0; i < rlGetScreenWidth()/200; i++) rlDrawRectangle(200*i, 0, 1, rlGetScreenHeight(), SKYBLUE);
            
            rlDrawCircle((int)position, rlGetScreenHeight()/2 - 25, 50, RED);
            
            rlDrawText(rlTextFormat("%03.0f ms", timeCounter*1000.0f), (int)position - 40, rlGetScreenHeight()/2 - 100, 20, MAROON);
            rlDrawText(rlTextFormat("PosX: %03.0f", position), (int)position - 50, rlGetScreenHeight()/2 + 40, 20, BLACK);
            
            rlDrawText("Circle is moving at a constant 200 pixels/sec,\nindependently of the frame rate.", 10, 10, 20, DARKGRAY);
            rlDrawText("PRESS SPACE to PAUSE MOVEMENT", 10, rlGetScreenHeight() - 60, 20, GRAY);
            rlDrawText("PRESS UP | DOWN to CHANGE TARGET FPS", 10, rlGetScreenHeight() - 30, 20, GRAY);
            rlDrawText(rlTextFormat("TARGET FPS: %i", targetFPS), rlGetScreenWidth() - 220, 10, 20, LIME);
            rlDrawText(rlTextFormat("CURRENT FPS: %i", (int)(1.0f/deltaTime)), rlGetScreenWidth() - 220, 40, 20, GREEN);

        rlEndDrawing();

        // NOTE: In case raylib is configured to SUPPORT_CUSTOM_FRAME_CONTROL, 
        // Events polling, screen buffer swap and frame time control must be managed by the user

        rlSwapScreenBuffer();         // Flip the back buffer to screen (front buffer)
        
        currentTime = rlGetTime();
        updateDrawTime = currentTime - previousTime;
        
        if (targetFPS > 0)          // We want a fixed frame rate
        {
            waitTime = (1.0f/(float)targetFPS) - updateDrawTime;
            if (waitTime > 0.0) 
            {
                rlWaitTime((float)waitTime);
                currentTime = rlGetTime();
                deltaTime = (float)(currentTime - previousTime);
            }
        }
        else deltaTime = (float)updateDrawTime;    // Framerate could be variable

        previousTime = currentTime;
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
