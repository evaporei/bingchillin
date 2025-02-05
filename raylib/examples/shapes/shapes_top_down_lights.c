/*******************************************************************************************
*
*   raylib [shapes] example - top down lights
*
*   Example originally created with raylib 4.2, last time updated with raylib 4.2
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2024 Jeffery Myers (@JeffM2501)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

// Custom Blend Modes
#define RLGL_SRC_ALPHA 0x0302
#define RLGL_MIN 0x8007
#define RLGL_MAX 0x8008

#define MAX_BOXES     20
#define MAX_SHADOWS   MAX_BOXES*3         // MAX_BOXES *3. Each box can cast up to two shadow volumes for the edges it is away from, and one for the box itself
#define MAX_LIGHTS    16

// Shadow geometry type
typedef struct ShadowGeometry {
    rlVector2 vertices[4];
} ShadowGeometry;

// Light info type
typedef struct LightInfo {
    bool active;                // Is this light slot active?
    bool dirty;                 // Does this light need to be updated?
    bool valid;                 // Is this light in a valid position?

    rlVector2 position;           // Light position
    rlRenderTexture mask;         // Alpha mask for the light
    float outerRadius;          // The distance the light touches
    rlRectangle bounds;           // A cached rectangle of the light bounds to help with culling

    ShadowGeometry shadows[MAX_SHADOWS];
    int shadowCount;
} LightInfo;


LightInfo lights[MAX_LIGHTS] = { 0 };

// Move a light and mark it as dirty so that we update it's mask next frame
void MoveLight(int slot, float x, float y)
{
    lights[slot].dirty = true;
    lights[slot].position.x = x; 
    lights[slot].position.y = y;

    // update the cached bounds
    lights[slot].bounds.x = x - lights[slot].outerRadius;
    lights[slot].bounds.y = y - lights[slot].outerRadius;
}

// Compute a shadow volume for the edge
// It takes the edge and projects it back by the light radius and turns it into a quad
void ComputeShadowVolumeForEdge(int slot, rlVector2 sp, rlVector2 ep)
{
    if (lights[slot].shadowCount >= MAX_SHADOWS) return;

    float extension = lights[slot].outerRadius*2;

    rlVector2 spVector = Vector2Normalize(Vector2Subtract(sp, lights[slot].position));
    rlVector2 spProjection = Vector2Add(sp, Vector2Scale(spVector, extension));

    rlVector2 epVector = Vector2Normalize(Vector2Subtract(ep, lights[slot].position));
    rlVector2 epProjection = Vector2Add(ep, Vector2Scale(epVector, extension));

    lights[slot].shadows[lights[slot].shadowCount].vertices[0] = sp;
    lights[slot].shadows[lights[slot].shadowCount].vertices[1] = ep;
    lights[slot].shadows[lights[slot].shadowCount].vertices[2] = epProjection;
    lights[slot].shadows[lights[slot].shadowCount].vertices[3] = spProjection;

    lights[slot].shadowCount++;
}

// Draw the light and shadows to the mask for a light
void DrawLightMask(int slot)
{
    // Use the light mask
    rlBeginTextureMode(lights[slot].mask);

        rlClearBackground(WHITE);

        // Force the blend mode to only set the alpha of the destination
        rlSetBlendFactors(RLGL_SRC_ALPHA, RLGL_SRC_ALPHA, RLGL_MIN);
        rlSetBlendMode(BLEND_CUSTOM);

        // If we are valid, then draw the light radius to the alpha mask
        if (lights[slot].valid) rlDrawCircleGradient((int)lights[slot].position.x, (int)lights[slot].position.y, lights[slot].outerRadius, rlColorAlpha(WHITE, 0), WHITE);
        
        rlDrawRenderBatchActive();

        // Cut out the shadows from the light radius by forcing the alpha to maximum
        rlSetBlendMode(BLEND_ALPHA);
        rlSetBlendFactors(RLGL_SRC_ALPHA, RLGL_SRC_ALPHA, RLGL_MAX);
        rlSetBlendMode(BLEND_CUSTOM);

        // Draw the shadows to the alpha mask
        for (int i = 0; i < lights[slot].shadowCount; i++)
        {
            rlDrawTriangleFan(lights[slot].shadows[i].vertices, 4, WHITE);
        }

        rlDrawRenderBatchActive();
        
        // Go back to normal blend mode
        rlSetBlendMode(BLEND_ALPHA);

    rlEndTextureMode();
}

// Setup a light
void SetupLight(int slot, float x, float y, float radius)
{
    lights[slot].active = true;
    lights[slot].valid = false;  // The light must prove it is valid
    lights[slot].mask = rlLoadRenderTexture(rlGetScreenWidth(), rlGetScreenHeight());
    lights[slot].outerRadius = radius;

    lights[slot].bounds.width = radius * 2;
    lights[slot].bounds.height = radius * 2;

    MoveLight(slot, x, y);

    // Force the render texture to have something in it
    DrawLightMask(slot);
}

// See if a light needs to update it's mask
bool UpdateLight(int slot, rlRectangle* boxes, int count)
{
    if (!lights[slot].active || !lights[slot].dirty) return false;

    lights[slot].dirty = false;
    lights[slot].shadowCount = 0;
    lights[slot].valid = false;

    for (int i = 0; i < count; i++)
    {
        // Are we in a box? if so we are not valid
        if (rlCheckCollisionPointRec(lights[slot].position, boxes[i])) return false;

        // If this box is outside our bounds, we can skip it
        if (!rlCheckCollisionRecs(lights[slot].bounds, boxes[i])) continue;

        // Check the edges that are on the same side we are, and cast shadow volumes out from them
        
        // Top
        rlVector2 sp = (rlVector2){ boxes[i].x, boxes[i].y };
        rlVector2 ep = (rlVector2){ boxes[i].x + boxes[i].width, boxes[i].y };

        if (lights[slot].position.y > ep.y) ComputeShadowVolumeForEdge(slot, sp, ep);

        // Right
        sp = ep;
        ep.y += boxes[i].height;
        if (lights[slot].position.x < ep.x) ComputeShadowVolumeForEdge(slot, sp, ep);

        // Bottom
        sp = ep;
        ep.x -= boxes[i].width;
        if (lights[slot].position.y < ep.y) ComputeShadowVolumeForEdge(slot, sp, ep);

        // Left
        sp = ep;
        ep.y -= boxes[i].height;
        if (lights[slot].position.x > ep.x) ComputeShadowVolumeForEdge(slot, sp, ep);

        // The box itself
        lights[slot].shadows[lights[slot].shadowCount].vertices[0] = (rlVector2){ boxes[i].x, boxes[i].y };
        lights[slot].shadows[lights[slot].shadowCount].vertices[1] = (rlVector2){ boxes[i].x, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices[2] = (rlVector2){ boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices[3] = (rlVector2){ boxes[i].x + boxes[i].width, boxes[i].y };
        lights[slot].shadowCount++;
    }

    lights[slot].valid = true;

    DrawLightMask(slot);

    return true;
}

// Set up some boxes
void SetupBoxes(rlRectangle *boxes, int *count)
{
    boxes[0] = (rlRectangle){ 150,80, 40, 40 };
    boxes[1] = (rlRectangle){ 1200, 700, 40, 40 };
    boxes[2] = (rlRectangle){ 200, 600, 40, 40 };
    boxes[3] = (rlRectangle){ 1000, 50, 40, 40 };
    boxes[4] = (rlRectangle){ 500, 350, 40, 40 };

    for (int i = 5; i < MAX_BOXES; i++)
    {
        boxes[i] = (rlRectangle){(float)rlGetRandomValue(0,rlGetScreenWidth()), (float)rlGetRandomValue(0,rlGetScreenHeight()), (float)rlGetRandomValue(10,100), (float)rlGetRandomValue(10,100) };
    }

    *count = MAX_BOXES;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - top down lights");

    // Initialize our 'world' of boxes
    int boxCount = 0;
    rlRectangle boxes[MAX_BOXES] = { 0 };
    SetupBoxes(boxes, &boxCount);

    // Create a checkerboard ground texture
    rlImage img = rlGenImageChecked(64, 64, 32, 32, DARKBROWN, DARKGRAY);
    Texture2D backgroundTexture = rlLoadTextureFromImage(img);
    rlUnloadImage(img);

    // Create a global light mask to hold all the blended lights
    rlRenderTexture lightMask = rlLoadRenderTexture(rlGetScreenWidth(), rlGetScreenHeight());

    // Setup initial light
    SetupLight(0, 600, 400, 300);
    int nextLight = 1;

    bool showLines = false;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Drag light 0
        if (rlIsMouseButtonDown(MOUSE_BUTTON_LEFT)) MoveLight(0, rlGetMousePosition().x, rlGetMousePosition().y);

        // Make a new light
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (nextLight < MAX_LIGHTS))
        {
            SetupLight(nextLight, rlGetMousePosition().x, rlGetMousePosition().y, 200);
            nextLight++;
        }

        // Toggle debug info
        if (rlIsKeyPressed(KEY_F1)) showLines = !showLines;

        // Update the lights and keep track if any were dirty so we know if we need to update the master light mask
        bool dirtyLights = false;
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            if (UpdateLight(i, boxes, boxCount)) dirtyLights = true;
        }

        // Update the light mask
        if (dirtyLights)
        {
            // Build up the light mask
            rlBeginTextureMode(lightMask);
            
                rlClearBackground(BLACK);

                // Force the blend mode to only set the alpha of the destination
                rlSetBlendFactors(RLGL_SRC_ALPHA, RLGL_SRC_ALPHA, RLGL_MIN);
                rlSetBlendMode(BLEND_CUSTOM);

                // Merge in all the light masks
                for (int i = 0; i < MAX_LIGHTS; i++)
                {
                    if (lights[i].active) rlDrawTextureRec(lights[i].mask.texture, (rlRectangle){ 0, 0, (float)rlGetScreenWidth(), -(float)rlGetScreenHeight() }, Vector2Zero(), WHITE);
                }

                rlDrawRenderBatchActive();

                // Go back to normal blend
                rlSetBlendMode(BLEND_ALPHA);
            rlEndTextureMode();
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(BLACK);
            
            // Draw the tile background
            rlDrawTextureRec(backgroundTexture, (rlRectangle){ 0, 0, (float)rlGetScreenWidth(), (float)rlGetScreenHeight() }, Vector2Zero(), WHITE);
            
            // Overlay the shadows from all the lights
            rlDrawTextureRec(lightMask.texture, (rlRectangle){ 0, 0, (float)rlGetScreenWidth(), -(float)rlGetScreenHeight() }, Vector2Zero(), rlColorAlpha(WHITE, showLines? 0.75f : 1.0f));

            // Draw the lights
            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                if (lights[i].active) rlDrawCircle((int)lights[i].position.x, (int)lights[i].position.y, 10, (i == 0)? YELLOW : WHITE);
            }

            if (showLines)
            {
                for (int s = 0; s < lights[0].shadowCount; s++)
                {
                    rlDrawTriangleFan(lights[0].shadows[s].vertices, 4, DARKPURPLE);
                }

                for (int b = 0; b < boxCount; b++)
                {
                    if (rlCheckCollisionRecs(boxes[b],lights[0].bounds)) rlDrawRectangleRec(boxes[b], PURPLE);

                    rlDrawRectangleLines((int)boxes[b].x, (int)boxes[b].y, (int)boxes[b].width, (int)boxes[b].height, DARKBLUE);
                }

                rlDrawText("(F1) Hide Shadow Volumes", 10, 50, 10, GREEN);
            }
            else
            {
                rlDrawText("(F1) Show Shadow Volumes", 10, 50, 10, GREEN);
            }

            rlDrawFPS(screenWidth - 80, 10);
            rlDrawText("Drag to move light #1", 10, 10, 10, DARKGREEN);
            rlDrawText("Right click to add new light", 10, 30, 10, DARKGREEN);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(backgroundTexture);
    rlUnloadRenderTexture(lightMask);
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].active) rlUnloadRenderTexture(lights[i].mask);
    }

    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}