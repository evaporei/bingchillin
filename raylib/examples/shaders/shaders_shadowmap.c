/*******************************************************************************************
*
*   raylib [shaders] example - Shadowmap
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.0
*
*   Example contributed by @TheManTheMythTheGameDev and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif

#define SHADOWMAP_RESOLUTION 1024

RenderTexture2D LoadShadowmapRenderTexture(int width, int height);
void UnloadShadowmapRenderTexture(RenderTexture2D target);
void DrawScene(rlModel cube, rlModel robot);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);
    // Shadows are a HUGE topic, and this example shows an extremely simple implementation of the shadowmapping algorithm,
    // which is the industry standard for shadows. This algorithm can be extended in a ridiculous number of ways to improve
    // realism and also adapt it for different scenes. This is pretty much the simplest possible implementation.
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - shadowmap");

    rlCamera3D cam = (rlCamera3D){ 0 };
    cam.position = (rlVector3){ 10.0f, 10.0f, 10.0f };
    cam.target = Vector3Zero();
    cam.projection = CAMERA_PERSPECTIVE;
    cam.up = (rlVector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;

    rlShader shadowShader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/shadowmap.vs", GLSL_VERSION),
                                     rlTextFormat("resources/shaders/glsl%i/shadowmap.fs", GLSL_VERSION));
    shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shadowShader, "viewPos");
    rlVector3 lightDir = Vector3Normalize((rlVector3){ 0.35f, -1.0f, -0.35f });
    rlColor lightColor = WHITE;
    rlVector4 lightColorNormalized = rlColorNormalize(lightColor);
    int lightDirLoc = rlGetShaderLocation(shadowShader, "lightDir");
    int lightColLoc = rlGetShaderLocation(shadowShader, "lightColor");
    rlSetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
    rlSetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    int ambientLoc = rlGetShaderLocation(shadowShader, "ambient");
    float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    rlSetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    int lightVPLoc = rlGetShaderLocation(shadowShader, "lightVP");
    int shadowMapLoc = rlGetShaderLocation(shadowShader, "shadowMap");
    int shadowMapResolution = SHADOWMAP_RESOLUTION;
    rlSetShaderValue(shadowShader, rlGetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);

    rlModel cube = rlLoadModelFromMesh(rlGenMeshCube(1.0f, 1.0f, 1.0f));
    cube.materials[0].shader = shadowShader;
    rlModel robot = rlLoadModel("resources/models/robot.glb");
    for (int i = 0; i < robot.materialCount; i++)
    {
        robot.materials[i].shader = shadowShader;
    }
    int animCount = 0;
    rlModelAnimation* robotAnimations = rlLoadModelAnimations("resources/models/robot.glb", &animCount);

    RenderTexture2D shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);
    // For the shadowmapping algorithm, we will be rendering everything from the light's point of view
    rlCamera3D lightCam = (rlCamera3D){ 0 };
    lightCam.position = Vector3Scale(lightDir, -15.0f);
    lightCam.target = Vector3Zero();
    // Use an orthographic projection for directional lights
    lightCam.projection = CAMERA_ORTHOGRAPHIC;
    lightCam.up = (rlVector3){ 0.0f, 1.0f, 0.0f };
    lightCam.fovy = 20.0f;

    rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    int fc = 0;

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float dt = rlGetFrameTime();

        rlVector3 cameraPos = cam.position;
        rlSetShaderValue(shadowShader, shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
        rlUpdateCamera(&cam, CAMERA_ORBITAL);

        fc++;
        fc %= (robotAnimations[0].frameCount);
        rlUpdateModelAnimation(robot, robotAnimations[0], fc);

        const float cameraSpeed = 0.05f;
        if (rlIsKeyDown(KEY_LEFT))
        {
            if (lightDir.x < 0.6f)
                lightDir.x += cameraSpeed * 60.0f * dt;
        }
        if (rlIsKeyDown(KEY_RIGHT))
        {
            if (lightDir.x > -0.6f)
                lightDir.x -= cameraSpeed * 60.0f * dt;
        }
        if (rlIsKeyDown(KEY_UP))
        {
            if (lightDir.z < 0.6f)
                lightDir.z += cameraSpeed * 60.0f * dt;
        }
        if (rlIsKeyDown(KEY_DOWN))
        {
            if (lightDir.z > -0.6f)
                lightDir.z -= cameraSpeed * 60.0f * dt;
        }
        lightDir = Vector3Normalize(lightDir);
        lightCam.position = Vector3Scale(lightDir, -15.0f);
        rlSetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

        // First, render all objects into the shadowmap
        // The idea is, we record all the objects' depths (as rendered from the light source's point of view) in a buffer
        // Anything that is "visible" to the light is in light, anything that isn't is in shadow
        // We can later use the depth buffer when rendering everything from the player's point of view
        // to determine whether a given point is "visible" to the light

        // Record the light matrices for future use!
        rlMatrix lightView;
        rlMatrix lightProj;
        rlBeginTextureMode(shadowMap);
        rlClearBackground(WHITE);
        rlBeginMode3D(lightCam);
            lightView = rlGetMatrixModelview();
            lightProj = rlGetMatrixProjection();
            DrawScene(cube, robot);
        rlEndMode3D();
        rlEndTextureMode();
        rlMatrix lightViewProj = MatrixMultiply(lightView, lightProj);

        rlClearBackground(RAYWHITE);

        rlSetShaderValueMatrix(shadowShader, lightVPLoc, lightViewProj);

        rlEnableShader(shadowShader.id);
        int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
        rlActiveTextureSlot(10);
        rlEnableTexture(shadowMap.depth.id);
        rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

        rlBeginMode3D(cam);

            // Draw the same exact things as we drew in the shadowmap!
            DrawScene(cube, robot);
        
        rlEndMode3D();

        rlDrawText("Shadows in raylib using the shadowmapping algorithm!", screenWidth - 320, screenHeight - 20, 10, GRAY);
        rlDrawText("Use the arrow keys to rotate the light!", 10, 10, 30, RED);

        rlEndDrawing();

        if (rlIsKeyPressed(KEY_F))
        {
            rlTakeScreenshot("shaders_shadowmap.png");
        }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    rlUnloadShader(shadowShader);
    rlUnloadModel(cube);
    rlUnloadModel(robot);
    rlUnloadModelAnimations(robotAnimations, animCount);
    UnloadShadowmapRenderTexture(shadowMap);

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // We don't need a color texture for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

// Unload shadowmap render texture from GPU memory (VRAM)
void UnloadShadowmapRenderTexture(RenderTexture2D target)
{
    if (target.id > 0)
    {
        // NOTE: Depth texture/renderbuffer is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}

void DrawScene(rlModel cube, rlModel robot)
{
    rlDrawModelEx(cube, Vector3Zero(), (rlVector3) { 0.0f, 1.0f, 0.0f }, 0.0f, (rlVector3) { 10.0f, 1.0f, 10.0f }, BLUE);
    rlDrawModelEx(cube, (rlVector3) { 1.5f, 1.0f, -1.5f }, (rlVector3) { 0.0f, 1.0f, 0.0f }, 0.0f, Vector3One(), WHITE);
    rlDrawModelEx(robot, (rlVector3) { 0.0f, 0.5f, 0.0f }, (rlVector3) { 0.0f, 1.0f, 0.0f }, 0.0f, (rlVector3) { 1.0f, 1.0f, 1.0f }, RED);
}
