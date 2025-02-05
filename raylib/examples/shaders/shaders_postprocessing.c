/*******************************************************************************************
*
*   raylib [shaders] example - Apply a postprocessing shader to a scene
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

#define MAX_POSTPRO_SHADERS         12

typedef enum {
    FX_GRAYSCALE = 0,
    FX_POSTERIZATION,
    FX_DREAM_VISION,
    FX_PIXELIZER,
    FX_CROSS_HATCHING,
    FX_CROSS_STITCHING,
    FX_PREDATOR_VIEW,
    FX_SCANLINES,
    FX_FISHEYE,
    FX_SOBEL,
    FX_BLOOM,
    FX_BLUR,
    //FX_FXAA
} PostproShader;

static const char *postproShaderText[] = {
    "GRAYSCALE",
    "POSTERIZATION",
    "DREAM_VISION",
    "PIXELIZER",
    "CROSS_HATCHING",
    "CROSS_STITCHING",
    "PREDATOR_VIEW",
    "SCANLINES",
    "FISHEYE",
    "SOBEL",
    "BLOOM",
    "BLUR",
    //"FXAA"
};

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

    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - postprocessing shader");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 2.0f, 3.0f, 2.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlModel model = rlLoadModel("resources/models/church.obj");                 // Load OBJ model
    Texture2D texture = LoadTexture("resources/models/church_diffuse.png"); // Load model texture (diffuse map)
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;        // Set model diffuse texture

    rlVector3 position = { 0.0f, 0.0f, 0.0f };            // Set model position

    // Load all postpro shaders
    // NOTE 1: All postpro shader use the base vertex shader (DEFAULT_VERTEX_SHADER)
    // NOTE 2: We load the correct shader depending on GLSL version
    rlShader shaders[MAX_POSTPRO_SHADERS] = { 0 };

    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    shaders[FX_GRAYSCALE] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
    shaders[FX_POSTERIZATION] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/posterization.fs", GLSL_VERSION));
    shaders[FX_DREAM_VISION] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/dream_vision.fs", GLSL_VERSION));
    shaders[FX_PIXELIZER] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/pixelizer.fs", GLSL_VERSION));
    shaders[FX_CROSS_HATCHING] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/cross_hatching.fs", GLSL_VERSION));
    shaders[FX_CROSS_STITCHING] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/cross_stitching.fs", GLSL_VERSION));
    shaders[FX_PREDATOR_VIEW] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/predator.fs", GLSL_VERSION));
    shaders[FX_SCANLINES] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/scanlines.fs", GLSL_VERSION));
    shaders[FX_FISHEYE] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/fisheye.fs", GLSL_VERSION));
    shaders[FX_SOBEL] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shaders[FX_BLOOM] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/bloom.fs", GLSL_VERSION));
    shaders[FX_BLUR] = rlLoadShader(0, rlTextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));

    int currentShader = FX_GRAYSCALE;

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

        if (rlIsKeyPressed(KEY_RIGHT)) currentShader++;
        else if (rlIsKeyPressed(KEY_LEFT)) currentShader--;

        if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
        else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginTextureMode(target);       // Enable drawing to texture
            rlClearBackground(RAYWHITE);  // Clear texture background

            rlBeginMode3D(camera);        // Begin 3d mode drawing
                rlDrawModel(model, position, 0.1f, WHITE);   // Draw 3d model with texture
                rlDrawGrid(10, 1.0f);     // Draw a grid
            rlEndMode3D();                // End 3d mode drawing, returns to orthographic 2d mode
        rlEndTextureMode();               // End drawing to texture (now we have a texture available for next passes)
        
        rlBeginDrawing();
            rlClearBackground(RAYWHITE);  // Clear screen background

            // Render generated texture using selected postprocessing shader
            rlBeginShaderMode(shaders[currentShader]);
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                rlDrawTextureRec(target.texture, (rlRectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (rlVector2){ 0, 0 }, WHITE);
            rlEndShaderMode();

            // Draw 2d shapes and text over drawn texture
            rlDrawRectangle(0, 9, 580, 30, rlFade(LIGHTGRAY, 0.7f));

            rlDrawText("(c) Church 3D model by Alberto Cano", screenWidth - 200, screenHeight - 20, 10, GRAY);
            rlDrawText("CURRENT POSTPRO SHADER:", 10, 15, 20, BLACK);
            rlDrawText(postproShaderText[currentShader], 330, 15, 20, RED);
            rlDrawText("< >", 540, 10, 30, DARKBLUE);
            rlDrawFPS(700, 15);
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload all postpro shaders
    for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) rlUnloadShader(shaders[i]);

    UnloadTexture(texture);         // Unload texture
    rlUnloadModel(model);             // Unload model
    rlUnloadRenderTexture(target);    // Unload render texture

    rlCloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
