/*******************************************************************************************
*
*   raylib [shaders] example - Julia sets
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example contributed by Josh Colclough (@joshcol9232) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Josh Colclough (@joshcol9232) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

// A few good julia sets
const float pointsOfInterest[6][2] =
{
    { -0.348827f, 0.607167f },
    { -0.786268f, 0.169728f },
    { -0.8f, 0.156f },
    { 0.285f, 0.0f },
    { -0.835f, -0.2321f },
    { -0.70176f, -0.3842f },
};

const int screenWidth = 800;
const int screenHeight = 450;
const float zoomSpeed = 1.01f;
const float offsetSpeedMul = 2.0f;

const float startingZoom = 0.75f;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - julia sets");

    // Load julia set shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/julia_set.fs", GLSL_VERSION));

    // Create a RenderTexture2D to be used for render to texture
    RenderTexture2D target = rlLoadRenderTexture(rlGetScreenWidth(), rlGetScreenHeight());

    // c constant to use in z^2 + c
    float c[2] = { pointsOfInterest[0][0], pointsOfInterest[0][1] };

    // Offset and zoom to draw the julia set at. (centered on screen and default size)
    float offset[2] = { 0.0f, 0.0f };
    float zoom = startingZoom;

    // Get variable (uniform) locations on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    int cLoc = rlGetShaderLocation(shader, "c");
    int zoomLoc = rlGetShaderLocation(shader, "zoom");
    int offsetLoc = rlGetShaderLocation(shader, "offset");

    // Upload the shader uniform values!
    rlSetShaderValue(shader, cLoc, c, SHADER_UNIFORM_VEC2);
    rlSetShaderValue(shader, zoomLoc, &zoom, SHADER_UNIFORM_FLOAT);
    rlSetShaderValue(shader, offsetLoc, offset, SHADER_UNIFORM_VEC2);

    int incrementSpeed = 0;             // Multiplier of speed to change c value
    bool showControls = true;           // Show controls

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Press [1 - 6] to reset c to a point of interest
        if (rlIsKeyPressed(KEY_ONE) ||
            rlIsKeyPressed(KEY_TWO) ||
            rlIsKeyPressed(KEY_THREE) ||
            rlIsKeyPressed(KEY_FOUR) ||
            rlIsKeyPressed(KEY_FIVE) ||
            rlIsKeyPressed(KEY_SIX))
        {
            if (rlIsKeyPressed(KEY_ONE)) c[0] = pointsOfInterest[0][0], c[1] = pointsOfInterest[0][1];
            else if (rlIsKeyPressed(KEY_TWO)) c[0] = pointsOfInterest[1][0], c[1] = pointsOfInterest[1][1];
            else if (rlIsKeyPressed(KEY_THREE)) c[0] = pointsOfInterest[2][0], c[1] = pointsOfInterest[2][1];
            else if (rlIsKeyPressed(KEY_FOUR)) c[0] = pointsOfInterest[3][0], c[1] = pointsOfInterest[3][1];
            else if (rlIsKeyPressed(KEY_FIVE)) c[0] = pointsOfInterest[4][0], c[1] = pointsOfInterest[4][1];
            else if (rlIsKeyPressed(KEY_SIX)) c[0] = pointsOfInterest[5][0], c[1] = pointsOfInterest[5][1];

            rlSetShaderValue(shader, cLoc, c, SHADER_UNIFORM_VEC2);
        }

        // If "R" is pressed, reset zoom and offset.
        if (rlIsKeyPressed(KEY_R))
        {
            zoom = startingZoom;
            offset[0] = 0.0f;
            offset[1] = 0.0f;
            rlSetShaderValue(shader, zoomLoc, &zoom, SHADER_UNIFORM_FLOAT);
            rlSetShaderValue(shader, offsetLoc, offset, SHADER_UNIFORM_VEC2);
        }

        if (rlIsKeyPressed(KEY_SPACE)) incrementSpeed = 0;         // Pause animation (c change)
        if (rlIsKeyPressed(KEY_F1)) showControls = !showControls;  // Toggle whether or not to show controls

        if (rlIsKeyPressed(KEY_RIGHT)) incrementSpeed++;
        else if (rlIsKeyPressed(KEY_LEFT)) incrementSpeed--;

        // If either left or right button is pressed, zoom in/out.
        if (rlIsMouseButtonDown(MOUSE_BUTTON_LEFT) || rlIsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            // Change zoom. If Mouse left -> zoom in. Mouse right -> zoom out.
            zoom *= rlIsMouseButtonDown(MOUSE_BUTTON_LEFT)? zoomSpeed : 1.0f/zoomSpeed;

            const rlVector2 mousePos = rlGetMousePosition();
            rlVector2 offsetVelocity;
            // Find the velocity at which to change the camera. Take the distance of the mouse
            // from the center of the screen as the direction, and adjust magnitude based on
            // the current zoom.
            offsetVelocity.x = (mousePos.x/(float)screenWidth - 0.5f)*offsetSpeedMul/zoom;
            offsetVelocity.y = (mousePos.y/(float)screenHeight - 0.5f)*offsetSpeedMul/zoom;

            // Apply move velocity to camera
            offset[0] += rlGetFrameTime()*offsetVelocity.x;
            offset[1] += rlGetFrameTime()*offsetVelocity.y;

            // Update the shader uniform values!
            rlSetShaderValue(shader, zoomLoc, &zoom, SHADER_UNIFORM_FLOAT);
            rlSetShaderValue(shader, offsetLoc, offset, SHADER_UNIFORM_VEC2);
        }

        // Increment c value with time
        const float dc = rlGetFrameTime()*(float)incrementSpeed*0.0005f;
        c[0] += dc;
        c[1] += dc;
        rlSetShaderValue(shader, cLoc, c, SHADER_UNIFORM_VEC2);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Using a render texture to draw Julia set
        rlBeginTextureMode(target);       // Enable drawing to texture
            rlClearBackground(BLACK);     // Clear the render texture

            // Draw a rectangle in shader mode to be used as shader canvas
            // NOTE: rlRectangle uses font white character texture coordinates,
            // so shader can not be applied here directly because input vertexTexCoord
            // do not represent full screen coordinates (space where want to apply shader)
            rlDrawRectangle(0, 0, rlGetScreenWidth(), rlGetScreenHeight(), BLACK);
        rlEndTextureMode();
            
        rlBeginDrawing();
            rlClearBackground(BLACK);     // Clear screen background

            // Draw the saved texture and rendered julia set with shader
            // NOTE: We do not invert texture on Y, already considered inside shader
            rlBeginShaderMode(shader);
                // WARNING: If FLAG_WINDOW_HIGHDPI is enabled, HighDPI monitor scaling should be considered
                // when rendering the RenderTexture2D to fit in the HighDPI scaled Window
                rlDrawTextureEx(target.texture, (rlVector2){ 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);
            rlEndShaderMode();

            if (showControls)
            {
                rlDrawText("Press Mouse buttons right/left to zoom in/out and move", 10, 15, 10, RAYWHITE);
                rlDrawText("Press KEY_F1 to toggle these controls", 10, 30, 10, RAYWHITE);
                rlDrawText("Press KEYS [1 - 6] to change point of interest", 10, 45, 10, RAYWHITE);
                rlDrawText("Press KEY_LEFT | KEY_RIGHT to change speed", 10, 60, 10, RAYWHITE);
                rlDrawText("Press KEY_SPACE to stop movement animation", 10, 75, 10, RAYWHITE);
                rlDrawText("Press KEY_R to recenter the camera", 10, 90, 10, RAYWHITE);
            }
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
