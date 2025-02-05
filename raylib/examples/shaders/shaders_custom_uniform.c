/*******************************************************************************************
*
*   raylib [shaders] example - Postprocessing with custom uniform variable
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 1.3, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - custom uniform variable");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 8.0f, 8.0f, 8.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 1.5f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlModel model = rlLoadModel("resources/models/barracks.obj");                   // Load OBJ model
    Texture2D texture = LoadTexture("resources/models/barracks_diffuse.png");   // Load model texture (diffuse map)
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;                     // Set model diffuse texture

    rlVector3 position = { 0.0f, 0.0f, 0.0f };                                    // Set model position

    // Load postprocessing shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    rlShader shader = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/swirl.fs", GLSL_VERSION));

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    int swirlCenterLoc = rlGetShaderLocation(shader, "center");

    float swirlCenter[2] = { (float)screenWidth/2, (float)screenHeight/2 };

    // Create a RenderTexture2D to be used for render to texture
    RenderTexture2D target = rlLoadRenderTexture(screenWidth, screenHeight);

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);
        
        rlVector2 mousePosition = rlGetMousePosition();

        swirlCenter[0] = mousePosition.x;
        swirlCenter[1] = screenHeight - mousePosition.y;

        // Send new value to the shader to be used on drawing
        rlSetShaderValue(shader, swirlCenterLoc, swirlCenter, SHADER_UNIFORM_VEC2);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginTextureMode(target);       // Enable drawing to texture
            rlClearBackground(RAYWHITE);  // Clear texture background

            rlBeginMode3D(camera);        // Begin 3d mode drawing
                rlDrawModel(model, position, 0.5f, WHITE);   // Draw 3d model with texture
                rlDrawGrid(10, 1.0f);     // Draw a grid
            rlEndMode3D();                // End 3d mode drawing, returns to orthographic 2d mode

            rlDrawText("TEXT DRAWN IN RENDER TEXTURE", 200, 10, 30, RED);
        rlEndTextureMode();               // End drawing to texture (now we have a texture available for next passes)

        rlBeginDrawing();
            rlClearBackground(RAYWHITE);  // Clear screen background

            // Enable shader using the custom uniform
            rlBeginShaderMode(shader);
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                rlDrawTextureRec(target.texture, (rlRectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (rlVector2){ 0, 0 }, WHITE);
            rlEndShaderMode();

            // Draw some 2d text over drawn texture
            rlDrawText("(c) Barracks 3D model by Alberto Cano", screenWidth - 220, screenHeight - 20, 10, GRAY);
            rlDrawFPS(10, 10);
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(shader);               // Unload shader
    UnloadTexture(texture);             // Unload texture
    rlUnloadModel(model);                 // Unload model
    rlUnloadRenderTexture(target);        // Unload render texture

    rlCloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}