/*******************************************************************************************
*
*   raylib [models] example - Skybox loading and drawing
*
*   Example originally created with raylib 1.8, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"      // Required for: MatrixPerspective(), MatrixLookAt()

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

// Generate cubemap (6 faces) from equirectangular (panorama) texture
static TextureCubemap GenTextureCubemap(rlShader shader, Texture2D panorama, int size, int format);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - skybox loading and drawing");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 1.0f, 1.0f, 1.0f };    // Camera position
    camera.target = (rlVector3){ 4.0f, 1.0f, 4.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load skybox model
    rlMesh cube = rlGenMeshCube(1.0f, 1.0f, 1.0f);
    rlModel skybox = rlLoadModelFromMesh(cube);

    // Set this to true to use an HDR rlTexture, Note that raylib must be built with HDR Support for this to work SUPPORT_FILEFORMAT_HDR
    bool useHDR = false;

    // Load skybox shader and set required locations
    // NOTE: Some locations are automatically set at shader loading
    skybox.materials[0].shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION),
                                            rlTextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION));

    rlSetShaderValue(skybox.materials[0].shader, rlGetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    rlSetShaderValue(skybox.materials[0].shader, rlGetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]) { useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);
    rlSetShaderValue(skybox.materials[0].shader, rlGetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){ useHDR ? 1 : 0 }, SHADER_UNIFORM_INT);

    // Load cubemap shader and setup required shader locations
    rlShader shdrCubemap = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
                                    rlTextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION));

    rlSetShaderValue(shdrCubemap, rlGetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

    char skyboxFileName[256] = { 0 };
    
    if (useHDR)
    {
        rlTextCopy(skyboxFileName, "resources/dresden_square_2k.hdr");

        // Load HDR panorama (sphere) texture
        Texture2D panorama = LoadTexture(skyboxFileName);

        // Generate cubemap (texture with 6 quads-cube-mapping) from panorama HDR texture
        // NOTE 1: New texture is generated rendering to texture, shader calculates the sphere->cube coordinates mapping
        // NOTE 2: It seems on some Android devices WebGL, fbo does not properly support a FLOAT-based attachment,
        // despite texture can be successfully created.. so using PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 instead of PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        UnloadTexture(panorama);        // rlTexture not required anymore, cubemap already generated
    }
    else
    {
        rlImage img = rlLoadImage("resources/skybox.png");
        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
        rlUnloadImage(img);
    }

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Load new cubemap texture on drag&drop
        if (rlIsFileDropped())
        {
            rlFilePathList droppedFiles = rlLoadDroppedFiles();

            if (droppedFiles.count == 1)         // Only support one file dropped
            {
                if (rlIsFileExtension(droppedFiles.paths[0], ".png;.jpg;.hdr;.bmp;.tga"))
                {
                    // Unload current cubemap texture to load new one
                    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
                    
                    if (useHDR)
                    {
                        // Load HDR panorama (sphere) texture
                        Texture2D panorama = LoadTexture(droppedFiles.paths[0]);

                        // Generate cubemap from panorama texture
                        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
                        
                        UnloadTexture(panorama);    // rlTexture not required anymore, cubemap already generated
                    }
                    else
                    {
                        rlImage img = rlLoadImage(droppedFiles.paths[0]);
                        skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
                        rlUnloadImage(img);
                    }

                    rlTextCopy(skyboxFileName, droppedFiles.paths[0]);
                }
            }

            rlUnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                // We are inside the cube, we need to disable backface culling!
                rlDisableBackfaceCulling();
                rlDisableDepthMask();
                    rlDrawModel(skybox, (rlVector3){0, 0, 0}, 1.0f, WHITE);
                rlEnableBackfaceCulling();
                rlEnableDepthMask();

                rlDrawGrid(10, 1.0f);

            rlEndMode3D();

            if (useHDR) rlDrawText(rlTextFormat("Panorama image from hdrihaven.com: %s", rlGetFileName(skyboxFileName)), 10, rlGetScreenHeight() - 20, 10, BLACK);
            else rlDrawText(rlTextFormat(": %s", rlGetFileName(skyboxFileName)), 10, rlGetScreenHeight() - 20, 10, BLACK);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

    rlUnloadModel(skybox);        // Unload skybox model

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Generate cubemap texture from HDR texture
static TextureCubemap GenTextureCubemap(rlShader shader, Texture2D panorama, int size, int format)
{
    TextureCubemap cubemap = { 0 };

    rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube

    // STEP 1: Setup framebuffer
    //------------------------------------------------------------------------------------------
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format);

    unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    // Check if framebuffer is complete with attachments (valid)
    if (rlFramebufferComplete(fbo)) rlTraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
    //------------------------------------------------------------------------------------------

    // STEP 2: Draw to framebuffer
    //------------------------------------------------------------------------------------------
    // NOTE: rlShader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
    rlEnableShader(shader.id);

    // Define projection matrix and send it to shader
    rlMatrix matFboProjection = MatrixPerspective(90.0*DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    // Define view matrix for every side of the cubemap
    rlMatrix fboViews[6] = {
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){  1.0f,  0.0f,  0.0f }, (rlVector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){ -1.0f,  0.0f,  0.0f }, (rlVector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){  0.0f,  1.0f,  0.0f }, (rlVector3){ 0.0f,  0.0f,  1.0f }),
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){  0.0f, -1.0f,  0.0f }, (rlVector3){ 0.0f,  0.0f, -1.0f }),
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){  0.0f,  0.0f,  1.0f }, (rlVector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector3){  0.0f,  0.0f, -1.0f }, (rlVector3){ 0.0f, -1.0f,  0.0f })
    };

    rlViewport(0, 0, size, size);   // Set viewport to current fbo dimensions
    
    // Activate and enable texture for drawing to cubemap faces
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++)
    {
        // Set the view matrix for the current cube face
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);
        
        // Select the current cubemap face attachment for the fbo
        // WARNING: This function by default enables->attach->disables fbo!!!
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        // Load and draw a cube, it uses the current enabled texture
        rlClearScreenBuffers();
        rlLoadDrawCube();

        // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
        // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
        // TODO: Investigate this issue...
        //rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
        //rlClearScreenBuffers();
        //rlDrawCubeV(Vector3Zero(), Vector3One(), WHITE);
        //rlDrawRenderBatchActive();
    }
    //------------------------------------------------------------------------------------------

    // STEP 3: Unload framebuffer and reset state
    //------------------------------------------------------------------------------------------
    rlDisableShader();          // Unbind shader
    rlDisableTexture();         // Unbind texture
    rlDisableFramebuffer();     // Unbind framebuffer
    rlUnloadFramebuffer(fbo);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)

    // Reset viewport dimensions to default
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();
    //------------------------------------------------------------------------------------------

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
}
