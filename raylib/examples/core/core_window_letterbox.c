/*******************************************************************************************
*
*   raylib [core] example - window scale letterbox (and virtual mouse)
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"        // Required for: Vector2Clamp()

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 450;

    // Enable config flags for resizable window and vertical synchro
    rlSetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    rlInitWindow(windowWidth, windowHeight, "raylib [core] example - window scale letterbox");
    rlSetWindowMinSize(320, 240);

    int gameScreenWidth = 640;
    int gameScreenHeight = 480;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = rlLoadRenderTexture(gameScreenWidth, gameScreenHeight);
    rlSetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // rlTexture scale filter to use

    rlColor colors[10] = { 0 };
    for (int i = 0; i < 10; i++) colors[i] = (rlColor){ rlGetRandomValue(100, 250), rlGetRandomValue(50, 150), rlGetRandomValue(10, 100), 255 };

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        float scale = MIN((float)rlGetScreenWidth()/gameScreenWidth, (float)rlGetScreenHeight()/gameScreenHeight);

        if (rlIsKeyPressed(KEY_SPACE))
        {
            // Recalculate random colors for the bars
            for (int i = 0; i < 10; i++) colors[i] = (rlColor){ rlGetRandomValue(100, 250), rlGetRandomValue(50, 150), rlGetRandomValue(10, 100), 255 };
        }

        // Update virtual mouse (clamped mouse value behind game screen)
        rlVector2 mouse = rlGetMousePosition();
        rlVector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (rlGetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (rlGetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (rlVector2){ 0, 0 }, (rlVector2){ (float)gameScreenWidth, (float)gameScreenHeight });

        // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
        //rlSetMouseOffset(-(rlGetScreenWidth() - (gameScreenWidth*scale))*0.5f, -(rlGetScreenHeight() - (gameScreenHeight*scale))*0.5f);
        //rlSetMouseScale(1/scale, 1/scale);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        rlBeginTextureMode(target);
            rlClearBackground(RAYWHITE);  // Clear render texture background color

            for (int i = 0; i < 10; i++) rlDrawRectangle(0, (gameScreenHeight/10)*i, gameScreenWidth, gameScreenHeight/10, colors[i]);

            rlDrawText("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, WHITE);
            rlDrawText(rlTextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, GREEN);
            rlDrawText(rlTextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);
        rlEndTextureMode();
        
        rlBeginDrawing();
            rlClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            rlDrawTexturePro(target.texture, (rlRectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (rlRectangle){ (rlGetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (rlGetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (rlVector2){ 0, 0 }, 0.0f, WHITE);
        rlEndDrawing();
        //--------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadRenderTexture(target);        // Unload render texture

    rlCloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
