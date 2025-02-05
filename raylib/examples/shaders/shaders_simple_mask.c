/*******************************************************************************************
*
*   raylib [shaders] example - Simple shader mask
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.7
*
*   Example contributed by Chris Camacho (@chriscamacho) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Chris Camacho (@chriscamacho) and Ramon Santamaria (@raysan5)
*
********************************************************************************************
*
*   After a model is loaded it has a default material, this material can be
*   modified in place rather than creating one from scratch...
*   While all of the maps have particular names, they can be used for any purpose
*   except for three maps that are applied as cubic maps (see below)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - simple shader mask");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 0.0f, 1.0f, 2.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Define our three models to show the shader on
    rlMesh torus = rlGenMeshTorus(0.3f, 1, 16, 32);
    rlModel model1 = rlLoadModelFromMesh(torus);

    rlMesh cube = rlGenMeshCube(0.8f,0.8f,0.8f);
    rlModel model2 = rlLoadModelFromMesh(cube);

    // Generate model to be shaded just to see the gaps in the other two
    rlMesh sphere = rlGenMeshSphere(1, 16, 16);
    rlModel model3 = rlLoadModelFromMesh(sphere);

    // Load the shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/mask.fs", GLSL_VERSION));

    // Load and apply the diffuse texture (colour map)
    rlTexture texDiffuse = LoadTexture("resources/plasma.png");
    model1.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texDiffuse;
    model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texDiffuse;

    // Using MATERIAL_MAP_EMISSION as a spare slot to use for 2nd texture
    // NOTE: Don't use MATERIAL_MAP_IRRADIANCE, MATERIAL_MAP_PREFILTER or  MATERIAL_MAP_CUBEMAP as they are bound as cube maps
    rlTexture texMask = LoadTexture("resources/mask.png");
    model1.materials[0].maps[MATERIAL_MAP_EMISSION].texture = texMask;
    model2.materials[0].maps[MATERIAL_MAP_EMISSION].texture = texMask;
    shader.locs[SHADER_LOC_MAP_EMISSION] = rlGetShaderLocation(shader, "mask");

    // Frame is incremented each frame to animate the shader
    int shaderFrame = rlGetShaderLocation(shader, "frame");

    // Apply the shader to the two models
    model1.materials[0].shader = shader;
    model2.materials[0].shader = shader;

    int framesCounter = 0;
    rlVector3 rotation = { 0 };           // rlModel rotation angles

    rlDisableCursor();                    // Limit cursor to relative movement inside the window
    rlSetTargetFPS(60);                   // Set  to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        framesCounter++;
        rotation.x += 0.01f;
        rotation.y += 0.005f;
        rotation.z -= 0.0025f;

        // Send frames counter to shader for animation
        rlSetShaderValue(shader, shaderFrame, &framesCounter, SHADER_UNIFORM_INT);

        // Rotate one of the models
        model1.transform = MatrixRotateXYZ(rotation);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(DARKBLUE);

            rlBeginMode3D(camera);

                rlDrawModel(model1, (rlVector3){ 0.5f, 0.0f, 0.0f }, 1, WHITE);
                rlDrawModelEx(model2, (rlVector3){ -0.5f, 0.0f, 0.0f }, (rlVector3){ 1.0f, 1.0f, 0.0f }, 50, (rlVector3){ 1.0f, 1.0f, 1.0f }, WHITE);
                rlDrawModel(model3,(rlVector3){ 0.0f, 0.0f, -1.5f }, 1, WHITE);
                rlDrawGrid(10, 1.0f);        // Draw a grid

            rlEndMode3D();

            rlDrawRectangle(16, 698, rlMeasureText(rlTextFormat("Frame: %i", framesCounter), 20) + 8, 42, BLUE);
            rlDrawText(rlTextFormat("Frame: %i", framesCounter), 20, 700, 20, WHITE);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(model1);
    rlUnloadModel(model2);
    rlUnloadModel(model3);

    UnloadTexture(texDiffuse);  // Unload default diffuse texture
    UnloadTexture(texMask);     // Unload texture mask

    rlUnloadShader(shader);       // Unload shader

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
