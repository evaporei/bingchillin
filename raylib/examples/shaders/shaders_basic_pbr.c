/*******************************************************************************************
*
*   raylib [shaders] example - Basic PBR
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.1-dev
*
*   Example contributed by Afan OLOVCIC (@_DevDad) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023-2024 Afan OLOVCIC (@_DevDad)
*
*   rlModel: "Old Rusty Car" (https://skfb.ly/LxRy) by Renafox, 
*   licensed under Creative Commons Attribution-NonCommercial 
*   (http://creativecommons.org/licenses/by-nc/4.0/)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            120
#endif

#include <stdlib.h>             // Required for: NULL

#define MAX_LIGHTS  4           // Max dynamic lights supported by shader

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Light type
typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT,
    LIGHT_SPOT
} LightType;

// Light data
typedef struct {
    int type;
    int enabled;
    rlVector3 position;
    rlVector3 target;
    float color[4];
    float intensity;

    // rlShader light parameters locations
    int typeLoc;
    int enabledLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int intensityLoc;
} Light;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static int lightCount = 0;     // Current number of dynamic lights that have been created

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
// Create a light and get shader locations
static Light CreateLight(int type, rlVector3 position, rlVector3 target, rlColor color, float intensity, rlShader shader);

// Update light properties on shader
// NOTE: Light shader locations should be available
static void UpdateLight(rlShader shader, Light light);

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT);
    rlInitWindow(screenWidth, screenHeight, "raylib [shaders] example - basic pbr");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 2.0f, 2.0f, 6.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 0.5f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load PBR shader and setup all required locations
    rlShader shader = rlLoadShader(rlTextFormat("resources/shaders/glsl%i/pbr.vs", GLSL_VERSION),
                               rlTextFormat("resources/shaders/glsl%i/pbr.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_MAP_ALBEDO] = rlGetShaderLocation(shader, "albedoMap");
    // WARNING: Metalness, roughness, and ambient occlusion are all packed into a MRA texture
    // They are passed as to the SHADER_LOC_MAP_METALNESS location for convenience,
    // shader already takes care of it accordingly
    shader.locs[SHADER_LOC_MAP_METALNESS] = rlGetShaderLocation(shader, "mraMap");
    shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetShaderLocation(shader, "normalMap");
    // WARNING: Similar to the MRA map, the emissive map packs different information 
    // into a single texture: it stores height and emission data
    // It is binded to SHADER_LOC_MAP_EMISSION location an properly processed on shader
    shader.locs[SHADER_LOC_MAP_EMISSION] = rlGetShaderLocation(shader, "emissiveMap");
    shader.locs[SHADER_LOC_COLOR_DIFFUSE] = rlGetShaderLocation(shader, "albedoColor");

    // Setup additional required shader locations, including lights data
    shader.locs[SHADER_LOC_VECTOR_VIEW] = rlGetShaderLocation(shader, "viewPos");
    int lightCountLoc = rlGetShaderLocation(shader, "numOfLights");
    int maxLightCount = MAX_LIGHTS;
    rlSetShaderValue(shader, lightCountLoc, &maxLightCount, SHADER_UNIFORM_INT);

    // Setup ambient color and intensity parameters
    float ambientIntensity = 0.02f;
    rlColor ambientColor = (rlColor){ 26, 32, 135, 255 };
    rlVector3 ambientColorNormalized = (rlVector3){ ambientColor.r/255.0f, ambientColor.g/255.0f, ambientColor.b/255.0f };
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "ambientColor"), &ambientColorNormalized, SHADER_UNIFORM_VEC3);
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "ambient"), &ambientIntensity, SHADER_UNIFORM_FLOAT);

    // Get location for shader parameters that can be modified in real time
    int emissiveIntensityLoc = rlGetShaderLocation(shader, "emissivePower");
    int emissiveColorLoc = rlGetShaderLocation(shader, "emissiveColor");
    int textureTilingLoc = rlGetShaderLocation(shader, "tiling");

    // Load old car model using PBR maps and shader
    // WARNING: We know this model consists of a single model.meshes[0] and
    // that model.materials[0] is by default assigned to that mesh
    // There could be more complex models consisting of multiple meshes and
    // multiple materials defined for those meshes... but always 1 mesh = 1 material
    rlModel car = rlLoadModel("resources/models/old_car_new.glb");

    // Assign already setup PBR shader to model.materials[0], used by models.meshes[0]
    car.materials[0].shader = shader;

    // Setup materials[0].maps default parameters
    car.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    car.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
    car.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
    car.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
    car.materials[0].maps[MATERIAL_MAP_EMISSION].color = (rlColor){ 255, 162, 0, 255 };

    // Setup materials[0].maps default textures
    car.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = LoadTexture("resources/old_car_d.png");
    car.materials[0].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture("resources/old_car_mra.png");
    car.materials[0].maps[MATERIAL_MAP_NORMAL].texture = LoadTexture("resources/old_car_n.png");
    car.materials[0].maps[MATERIAL_MAP_EMISSION].texture = LoadTexture("resources/old_car_e.png");
    
    // Load floor model mesh and assign material parameters
    // NOTE: A basic plane shape can be generated instead of being loaded from a model file
    rlModel floor = rlLoadModel("resources/models/plane.glb");
    //rlMesh floorMesh = rlGenMeshPlane(10, 10, 10, 10);
    //rlGenMeshTangents(&floorMesh);      // TODO: Review tangents generation
    //rlModel floor = rlLoadModelFromMesh(floorMesh);

    // Assign material shader for our floor model, same PBR shader 
    floor.materials[0].shader = shader;
    
    floor.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    floor.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
    floor.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
    floor.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
    floor.materials[0].maps[MATERIAL_MAP_EMISSION].color = BLACK;

    floor.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = LoadTexture("resources/road_a.png");
    floor.materials[0].maps[MATERIAL_MAP_METALNESS].texture = LoadTexture("resources/road_mra.png");
    floor.materials[0].maps[MATERIAL_MAP_NORMAL].texture = LoadTexture("resources/road_n.png");

    // Models texture tiling parameter can be stored in the rlMaterial struct if required (CURRENTLY NOT USED)
    // NOTE: rlMaterial.params[4] are available for generic parameters storage (float)
    rlVector2 carTextureTiling = (rlVector2){ 0.5f, 0.5f };
    rlVector2 floorTextureTiling = (rlVector2){ 0.5f, 0.5f };

    // Create some lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (rlVector3){ -1.0f, 1.0f, -2.0f }, (rlVector3){ 0.0f, 0.0f, 0.0f }, YELLOW, 4.0f, shader);
    lights[1] = CreateLight(LIGHT_POINT, (rlVector3){ 2.0f, 1.0f, 1.0f }, (rlVector3){ 0.0f, 0.0f, 0.0f }, GREEN, 3.3f, shader);
    lights[2] = CreateLight(LIGHT_POINT, (rlVector3){ -2.0f, 1.0f, 1.0f }, (rlVector3){ 0.0f, 0.0f, 0.0f }, RED, 8.3f, shader);
    lights[3] = CreateLight(LIGHT_POINT, (rlVector3){ 1.0f, 1.0f, -2.0f }, (rlVector3){ 0.0f, 0.0f, 0.0f }, BLUE, 2.0f, shader);

    // Setup material texture maps usage in shader
    // NOTE: By default, the texture maps are always used
    int usage = 1;
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "useTexAlbedo"), &usage, SHADER_UNIFORM_INT);
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "useTexNormal"), &usage, SHADER_UNIFORM_INT);
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "useTexMRA"), &usage, SHADER_UNIFORM_INT);
    rlSetShaderValue(shader, rlGetShaderLocation(shader, "useTexEmissive"), &usage, SHADER_UNIFORM_INT);
    
    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
        float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        rlSetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Check key inputs to enable/disable lights
        if (rlIsKeyPressed(KEY_ONE)) { lights[2].enabled = !lights[2].enabled; }
        if (rlIsKeyPressed(KEY_TWO)) { lights[1].enabled = !lights[1].enabled; }
        if (rlIsKeyPressed(KEY_THREE)) { lights[3].enabled = !lights[3].enabled; }
        if (rlIsKeyPressed(KEY_FOUR)) { lights[0].enabled = !lights[0].enabled; }

        // Update light values on shader (actually, only enable/disable them)
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLight(shader, lights[i]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();
        
            rlClearBackground(BLACK);
            
            rlBeginMode3D(camera);
                
                // Set floor model texture tiling and emissive color parameters on shader
                rlSetShaderValue(shader, textureTilingLoc, &floorTextureTiling, SHADER_UNIFORM_VEC2);
                rlVector4 floorEmissiveColor = rlColorNormalize(floor.materials[0].maps[MATERIAL_MAP_EMISSION].color);
                rlSetShaderValue(shader, emissiveColorLoc, &floorEmissiveColor, SHADER_UNIFORM_VEC4);
                
                rlDrawModel(floor, (rlVector3){ 0.0f, 0.0f, 0.0f }, 5.0f, WHITE);   // Draw floor model

                // Set old car model texture tiling, emissive color and emissive intensity parameters on shader
                rlSetShaderValue(shader, textureTilingLoc, &carTextureTiling, SHADER_UNIFORM_VEC2);
                rlVector4 carEmissiveColor = rlColorNormalize(car.materials[0].maps[MATERIAL_MAP_EMISSION].color);
                rlSetShaderValue(shader, emissiveColorLoc, &carEmissiveColor, SHADER_UNIFORM_VEC4);
                float emissiveIntensity = 0.01f;
                rlSetShaderValue(shader, emissiveIntensityLoc, &emissiveIntensity, SHADER_UNIFORM_FLOAT);
                
                rlDrawModel(car, (rlVector3){ 0.0f, 0.0f, 0.0f }, 0.25f, WHITE);   // Draw car model

                // Draw spheres to show the lights positions
                for (int i = 0; i < MAX_LIGHTS; i++)
                {
                    rlColor lightColor = (rlColor){ lights[i].color[0]*255, lights[i].color[1]*255, lights[i].color[2]*255, lights[i].color[3]*255 };
                    
                    if (lights[i].enabled) rlDrawSphereEx(lights[i].position, 0.2f, 8, 8, lightColor);
                    else rlDrawSphereWires(lights[i].position, 0.2f, 8, 8, rlColorAlpha(lightColor, 0.3f));
                }
                
            rlEndMode3D();
            
            rlDrawText("Toggle lights: [1][2][3][4]", 10, 40, 20, LIGHTGRAY);

            rlDrawText("(c) Old Rusty Car model by Renafox (https://skfb.ly/LxRy)", screenWidth - 320, screenHeight - 20, 10, LIGHTGRAY);
            
            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unbind (disconnect) shader from car.material[0] 
    // to avoid rlUnloadMaterial() trying to unload it automatically
    car.materials[0].shader = (rlShader){ 0 };
    rlUnloadMaterial(car.materials[0]);
    car.materials[0].maps = NULL;
    rlUnloadModel(car);
    
    floor.materials[0].shader = (rlShader){ 0 };
    rlUnloadMaterial(floor.materials[0]);
    floor.materials[0].maps = NULL;
    rlUnloadModel(floor);
    
    rlUnloadShader(shader);       // Unload rlShader
    
    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Create light with provided data
// NOTE: It updated the global lightCount and it's limited to MAX_LIGHTS
static Light CreateLight(int type, rlVector3 position, rlVector3 target, rlColor color, float intensity, rlShader shader)
{
    Light light = { 0 };

    if (lightCount < MAX_LIGHTS)
    {
        light.enabled = 1;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color[0] = (float)color.r/255.0f;
        light.color[1] = (float)color.g/255.0f;
        light.color[2] = (float)color.b/255.0f;
        light.color[3] = (float)color.a/255.0f;
        light.intensity = intensity;
        
        // NOTE: rlShader parameters names for lights must match the requested ones
        light.enabledLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].enabled", lightCount));
        light.typeLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].type", lightCount));
        light.positionLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].position", lightCount));
        light.targetLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].target", lightCount));
        light.colorLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].color", lightCount));
        light.intensityLoc = rlGetShaderLocation(shader, rlTextFormat("lights[%i].intensity", lightCount));
        
        UpdateLight(shader, light);

        lightCount++;
    }

    return light;
}

// Send light properties to shader
// NOTE: Light shader locations should be available
static void UpdateLight(rlShader shader, Light light)
{
    rlSetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    rlSetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);
    
    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    rlSetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    rlSetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);
    rlSetShaderValue(shader, light.colorLoc, light.color, SHADER_UNIFORM_VEC4);
    rlSetShaderValue(shader, light.intensityLoc, &light.intensity, SHADER_UNIFORM_FLOAT);
}
