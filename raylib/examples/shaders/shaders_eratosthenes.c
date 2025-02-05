/*******************************************************************************************
*
*   raylib [shaders] example - Sieve of Eratosthenes
*
*   NOTE: Sieve of Eratosthenes, the earliest known (ancient Greek) prime number sieve.
*
*       "Sift the twos and sift the threes,
*        The Sieve of Eratosthenes.
*        When the multiples sublime,
*        the numbers that are left are prime."
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example contributed by ProfJski and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 ProfJski and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - Sieve of Eratosthenes");

    RenderTexture2D target = rlLoadRenderTexture(screenWidth, screenHeight);

    // Load Eratosthenes shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/eratosthenes.fs", GLSL_VERSION));

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Nothing to do here, everything is happening in the shader
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginTextureMode(target);       // Enable drawing to texture
            rlClearBackground(BLACK);     // Clear the render texture

            // Draw a rectangle in shader mode to be used as shader canvas
            // NOTE: rlRectangle uses font white character texture coordinates,
            // so shader can not be applied here directly because input vertexTexCoord
            // do not represent full screen coordinates (space where want to apply shader)
            rlDrawRectangle(0, 0, rlGetScreenWidth(), rlGetScreenHeight(), BLACK);
        rlEndTextureMode();               // End drawing to texture (now we have a blank texture available for the shader)

        rlBeginDrawing();
            rlClearBackground(RAYWHITE);  // Clear screen background

            rlBeginShaderMode(shader);
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                rlDrawTextureRec(target.texture, (rlRectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (rlVector2){ 0.0f, 0.0f }, WHITE);
            rlEndShaderMode();
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);               // Unload shader
    rlUnloadRenderTexture(target);        // Unload render texture

    rlCloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
