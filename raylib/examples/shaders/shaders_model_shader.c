/*******************************************************************************************
*
*   raylib [shaders] example - rlModel shader
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 1.3, last time updated with raylib 3.7
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
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

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);      // Enable Multi Sampling Anti Aliasing 4x (if available)

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - model shader");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 4.0f, 4.0f, 4.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 1.0f, -1.0f };     // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlModel model = rlLoadModel("resources/models/watermill.obj");                   // Load OBJ model
    Texture2D texture = LoadTexture("resources/models/watermill_diffuse.png");   // Load model texture

    // Load shader for model
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));

    model.materials[0].shader = shader;                     // Set shader effect to 3d model
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Bind texture to model

    rlVector3 position = { 0.0f, 0.0f, 0.0f };    // Set model position

    rlDisableCursor();                    // Limit cursor to relative movement inside the window
    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawModel(model, position, 0.2f, WHITE);   // Draw 3d model with texture

                rlDrawGrid(10, 1.0f);     // Draw a grid

            rlEndMode3D();

            rlDrawText("(c) Watermill 3D model by Alberto Cano", screenWidth - 210, screenHeight - 20, 10, GRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);       // Unload shader
    UnloadTexture(texture);     // Unload texture
    rlUnloadModel(model);         // Unload model

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}