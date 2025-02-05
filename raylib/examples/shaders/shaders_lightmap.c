/*******************************************************************************************
*
*   raylib [shaders] example - lightmap
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3).
*
*   Example contributed by Jussi Viitala (@nullstare) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Jussi Viitala (@nullstare) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAP_SIZE 10

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - lightmap");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 4.0f, 6.0f, 8.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlMesh mesh = rlGenMeshPlane((float)MAP_SIZE, (float)MAP_SIZE, 1, 1);

    // rlGenMeshPlane doesn't generate texcoords2 so we will upload them separately
    mesh.texcoords2 = (float *)RL_MALLOC(mesh.vertexCount*2*sizeof(float));

    // X                          // Y
    mesh.texcoords2[0] = 0.0f;    mesh.texcoords2[1] = 0.0f;
    mesh.texcoords2[2] = 1.0f;    mesh.texcoords2[3] = 0.0f;
    mesh.texcoords2[4] = 0.0f;    mesh.texcoords2[5] = 1.0f;
    mesh.texcoords2[6] = 1.0f;    mesh.texcoords2[7] = 1.0f;

    // Load a new texcoords2 attributes buffer
    mesh.vboId[SHADER_LOC_VERTEX_TEXCOORD02] = rlLoadVertexBuffer(mesh.texcoords2, mesh.vertexCount*2*sizeof(float), false);
    rlEnableVertexArray(mesh.vaoId);
    
    // Index 5 is for texcoords2
    rlSetVertexAttribute(5, 2, RL_FLOAT, 0, 0, 0);
    rlEnableVertexAttribute(5);
    rlDisableVertexArray();

    // Load lightmap shader
    rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/lightmap.vs", GLSL_VERSION),
                               rlTextFormat("resources/shaders/glsl%i/lightmap.fs", GLSL_VERSION));

    rlTexture texture = LoadTexture("resources/cubicmap_atlas.png");
    rlTexture light = LoadTexture("resources/spark_flame.png");

    GenTextureMipmaps(&texture);
    rlSetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);

    rlRenderTexture lightmap = rlLoadRenderTexture(MAP_SIZE, MAP_SIZE);

    rlSetTextureFilter(lightmap.texture, TEXTURE_FILTER_TRILINEAR);

    rlMaterial material = rlLoadMaterialDefault();
    material.shader = shader;
    material.maps[MATERIAL_MAP_ALBEDO].texture = texture;
    material.maps[MATERIAL_MAP_METALNESS].texture = lightmap.texture;

    // Drawing to lightmap
    rlBeginTextureMode(lightmap);
        rlClearBackground(BLACK);

        rlBeginBlendMode(BLEND_ADDITIVE);
            rlDrawTexturePro(
                light,
                (rlRectangle){ 0, 0, light.width, light.height },
                (rlRectangle){ 0, 0, 20, 20 },
                (rlVector2){ 10.0, 10.0 },
                0.0,
                RED
            );
            rlDrawTexturePro(
                light,
                (rlRectangle){ 0, 0, light.width, light.height },
                (rlRectangle){ 8, 4, 20, 20 },
                (rlVector2){ 10.0, 10.0 },
                0.0,
                BLUE
            );
            rlDrawTexturePro(
                light,
                (rlRectangle){ 0, 0, light.width, light.height },
                (rlRectangle){ 8, 8, 10, 10 },
                (rlVector2){ 5.0, 5.0 },
                0.0,
                GREEN
            );
        rlBeginBlendMode(BLEND_ALPHA);
    rlEndTextureMode();

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();
            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
                rlDrawMesh(mesh, material, MatrixIdentity());
            rlEndMode3D();

            rlDrawFPS(10, 10);

            rlDrawTexturePro(
                lightmap.texture,
                (rlRectangle){ 0, 0, -MAP_SIZE, -MAP_SIZE },
                (rlRectangle){ rlGetRenderWidth() - MAP_SIZE*8 - 10, 10, MAP_SIZE*8, MAP_SIZE*8 },
                (rlVector2){ 0.0, 0.0 },
                0.0,
                WHITE);
                
            rlDrawText("lightmap", rlGetRenderWidth() - 66, 16 + MAP_SIZE*8, 10, GRAY);
            rlDrawText("10x10 pixels", rlGetRenderWidth() - 76, 30 + MAP_SIZE*8, 10, GRAY);
                
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadMesh(mesh);       // Unload the mesh
    rlUnloadShader(shader);   // Unload shader
    UnloadTexture(texture); // Unload texture
    UnloadTexture(light);   // Unload texture

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
