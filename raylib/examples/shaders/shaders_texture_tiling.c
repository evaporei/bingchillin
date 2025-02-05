/*******************************************************************************************
*
*   raylib [shaders] example - texture tiling
*
*   Example demonstrates how to tile a texture on a 3D model using raylib.
*
*   Example contributed by Luis Almeida (@luis605) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Luis Almeida (@luis605)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture tiling");

    // Define the camera to look into our 3d world
    rlCamera3D camera = { 0 };
    camera.position = (rlVector3){ 4.0f, 4.0f, 4.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 0.5f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load a cube model
    rlMesh cube = rlGenMeshCube(1.0f, 1.0f, 1.0f);
    rlModel model = rlLoadModelFromMesh(cube);
    
    // Load a texture and assign to cube model
    Texture2D texture = LoadTexture("resources/cubicmap_atlas.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Set the texture tiling using a shader
    float tiling[2] = { 3.0f, 3.0f };
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/tiling.fs", GLSL_VERSION));
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "tiling"), tiling, SHADER_UNIFORM_VEC2);
    model.materials[0].shader = shader;

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FREE);

        if (rlIsKeyPressed('Z')) camera.target = (rlVector3){ 0.0f, 0.5f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();
        
            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
            
                rlBeginShaderMode(shader);
                    rlDrawModel(model, (rlVector3){ 0.0f, 0.0f, 0.0f }, 2.0f, WHITE);
                rlEndShaderMode();

                rlDrawGrid(10, 1.0f);
                
            rlEndMode3D();

            rlDrawText("Use mouse to rotate the camera", 10, 10, 20, DARKGRAY);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(model);         // Unload model
    rlUnloadShader(shader);       // Unload shader
    UnloadTexture(texture);     // Unload texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}
