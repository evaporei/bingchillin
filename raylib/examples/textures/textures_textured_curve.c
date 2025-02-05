/*******************************************************************************************
*
*   raylib [textures] example - Draw a texture along a segmented curve
*
*   Example originally created with raylib 4.5, last time updated with raylib 4.5
*
*   Example contributed by Jeffery Myers and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2024 Jeffery Myers and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "raymath.h"
#include "rlgl.h"

#include <math.h>           // Required for: powf()
#include <stdlib.h>         // Required for: NULL

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static rlTexture texRoad = { 0 };

static bool showCurve = false;

static float curveWidth = 50;
static int curveSegments = 24;

static rlVector2 curveStartPosition = { 0 };
static rlVector2 curveStartPositionTangent = { 0 };

static rlVector2 curveEndPosition = { 0 };
static rlVector2 curveEndPositionTangent = { 0 };

static rlVector2 *curveSelectedPoint = NULL;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void DrawTexturedCurve(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    rlInitWindow(screenWidth, screenHeight, "raylib [textures] examples - textured curve");

    // Load the road texture
    texRoad = LoadTexture("resources/road.png");
    rlSetTextureFilter(texRoad, TEXTURE_FILTER_BILINEAR);

    // Setup the curve
    curveStartPosition = (rlVector2){ 80, 100 };
    curveStartPositionTangent = (rlVector2){ 100, 300 };

    curveEndPosition = (rlVector2){ 700, 350 };
    curveEndPositionTangent = (rlVector2){ 600, 100 };

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Curve config options
        if (rlIsKeyPressed(KEY_SPACE)) showCurve = !showCurve;
        if (rlIsKeyPressed(KEY_EQUAL)) curveWidth += 2;
        if (rlIsKeyPressed(KEY_MINUS)) curveWidth -= 2;
        if (curveWidth < 2) curveWidth = 2;

        // Update segments
        if (rlIsKeyPressed(KEY_LEFT)) curveSegments -= 2;
        if (rlIsKeyPressed(KEY_RIGHT)) curveSegments += 2;

        if (curveSegments < 2) curveSegments = 2;

        // Update curve logic
        // If the mouse is not down, we are not editing the curve so clear the selection
        if (!rlIsMouseButtonDown(MOUSE_LEFT_BUTTON))  curveSelectedPoint = NULL;

        // If a point was selected, move it
        if (curveSelectedPoint) *curveSelectedPoint = Vector2Add(*curveSelectedPoint, rlGetMouseDelta());

        // The mouse is down, and nothing was selected, so see if anything was picked
        rlVector2 mouse = rlGetMousePosition();
        if (rlCheckCollisionPointCircle(mouse, curveStartPosition, 6)) curveSelectedPoint = &curveStartPosition;
        else if (rlCheckCollisionPointCircle(mouse, curveStartPositionTangent, 6)) curveSelectedPoint = &curveStartPositionTangent;
        else if (rlCheckCollisionPointCircle(mouse, curveEndPosition, 6)) curveSelectedPoint = &curveEndPosition;
        else if (rlCheckCollisionPointCircle(mouse, curveEndPositionTangent, 6)) curveSelectedPoint = &curveEndPositionTangent;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            DrawTexturedCurve();    // Draw a textured Spline Cubic Bezier
            
            // Draw spline for reference
            if (showCurve) rlDrawSplineSegmentBezierCubic(curveStartPosition, curveEndPosition, curveStartPositionTangent, curveEndPositionTangent, 2, BLUE);

            // Draw the various control points and highlight where the mouse is
            rlDrawLineV(curveStartPosition, curveStartPositionTangent, SKYBLUE);
            rlDrawLineV(curveStartPositionTangent, curveEndPositionTangent, rlFade(LIGHTGRAY, 0.4f));
            rlDrawLineV(curveEndPosition, curveEndPositionTangent, PURPLE);
            
            if (rlCheckCollisionPointCircle(mouse, curveStartPosition, 6)) rlDrawCircleV(curveStartPosition, 7, YELLOW);
            rlDrawCircleV(curveStartPosition, 5, RED);

            if (rlCheckCollisionPointCircle(mouse, curveStartPositionTangent, 6)) rlDrawCircleV(curveStartPositionTangent, 7, YELLOW);
            rlDrawCircleV(curveStartPositionTangent, 5, MAROON);

            if (rlCheckCollisionPointCircle(mouse, curveEndPosition, 6)) rlDrawCircleV(curveEndPosition, 7, YELLOW);
            rlDrawCircleV(curveEndPosition, 5, GREEN);

            if (rlCheckCollisionPointCircle(mouse, curveEndPositionTangent, 6)) rlDrawCircleV(curveEndPositionTangent, 7, YELLOW);
            rlDrawCircleV(curveEndPositionTangent, 5, DARKGREEN);

            // Draw usage info
            rlDrawText("Drag points to move curve, press SPACE to show/hide base curve", 10, 10, 10, DARKGRAY);
            rlDrawText(rlTextFormat("Curve width: %2.0f (Use + and - to adjust)", curveWidth), 10, 30, 10, DARKGRAY);
            rlDrawText(rlTextFormat("Curve segments: %d (Use LEFT and RIGHT to adjust)", curveSegments), 10, 50, 10, DARKGRAY);
            
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texRoad);
    
    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Draw textured curve using Spline Cubic Bezier
static void DrawTexturedCurve(void)
{
    const float step = 1.0f/curveSegments;

    rlVector2 previous = curveStartPosition;
    rlVector2 previousTangent = { 0 };
    float previousV = 0;

    // We can't compute a tangent for the first point, so we need to reuse the tangent from the first segment
    bool tangentSet = false;

    rlVector2 current = { 0 };
    float t = 0.0f;

    for (int i = 1; i <= curveSegments; i++)
    {
        t = step*(float)i;

        float a = powf(1.0f - t, 3);
        float b = 3.0f*powf(1.0f - t, 2)*t;
        float c = 3.0f*(1.0f - t)*powf(t, 2);
        float d = powf(t, 3);

        // Compute the endpoint for this segment
        current.y = a*curveStartPosition.y + b*curveStartPositionTangent.y + c*curveEndPositionTangent.y + d*curveEndPosition.y;
        current.x = a*curveStartPosition.x + b*curveStartPositionTangent.x + c*curveEndPositionTangent.x + d*curveEndPosition.x;

        // Vector from previous to current
        rlVector2 delta = { current.x - previous.x, current.y - previous.y };

        // The right hand normal to the delta vector
        rlVector2 normal = Vector2Normalize((rlVector2){ -delta.y, delta.x });

        // The v texture coordinate of the segment (add up the length of all the segments so far)
        float v = previousV + Vector2Length(delta);

        // Make sure the start point has a normal
        if (!tangentSet)
        {
            previousTangent = normal;
            tangentSet = true;
        }

        // Extend out the normals from the previous and current points to get the quad for this segment
        rlVector2 prevPosNormal = Vector2Add(previous, Vector2Scale(previousTangent, curveWidth));
        rlVector2 prevNegNormal = Vector2Add(previous, Vector2Scale(previousTangent, -curveWidth));

        rlVector2 currentPosNormal = Vector2Add(current, Vector2Scale(normal, curveWidth));
        rlVector2 currentNegNormal = Vector2Add(current, Vector2Scale(normal, -curveWidth));

        // Draw the segment as a quad
        rlSetTexture(texRoad.id);
        rlBegin(RL_QUADS);
            rlColor4ub(255,255,255,255);
            rlNormal3f(0.0f, 0.0f, 1.0f);

            rlTexCoord2f(0, previousV);
            rlVertex2f(prevNegNormal.x, prevNegNormal.y);

            rlTexCoord2f(1, previousV);
            rlVertex2f(prevPosNormal.x, prevPosNormal.y);

            rlTexCoord2f(1, v);
            rlVertex2f(currentPosNormal.x, currentPosNormal.y);

            rlTexCoord2f(0, v);
            rlVertex2f(currentNegNormal.x, currentNegNormal.y);
        rlEnd();

        // The current step is the start of the next step
        previous = current;
        previousTangent = normal;
        previousV = v;
    }
}

