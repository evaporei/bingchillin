/*******************************************************************************************
*
*   raylib [shapes] example - splines drawing
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"     // Required for UI controls

#include <stdlib.h>     // Required for: NULL

#define MAX_SPLINE_POINTS      32

// Cubic Bezier spline control points
// NOTE: Every segment has two control points 
typedef struct {
    rlVector2 start;
    rlVector2 end;
} ControlPoint;

// Spline types
typedef enum {
    SPLINE_LINEAR = 0,      // Linear
    SPLINE_BASIS,           // B-Spline
    SPLINE_CATMULLROM,      // Catmull-Rom
    SPLINE_BEZIER           // Cubic Bezier
} SplineType;

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
    rlInitWindow(screenWidth, screenHeight, "raylib [shapes] example - splines drawing");

    rlVector2 points[MAX_SPLINE_POINTS] = {
        {  50.0f, 400.0f },
        { 160.0f, 220.0f },
        { 340.0f, 380.0f },
        { 520.0f, 60.0f },
        { 710.0f, 260.0f },
    };
    
    // Array required for spline bezier-cubic, 
    // including control points interleaved with start-end segment points
    rlVector2 pointsInterleaved[3*(MAX_SPLINE_POINTS - 1) + 1] = { 0 };
    
    int pointCount = 5;
    int selectedPoint = -1;
    int focusedPoint = -1;
    rlVector2 *selectedControlPoint = NULL;
    rlVector2 *focusedControlPoint = NULL;
    
    // Cubic Bezier control points initialization
    ControlPoint control[MAX_SPLINE_POINTS-1] = { 0 };
    for (int i = 0; i < pointCount - 1; i++)
    {
        control[i].start = (rlVector2){ points[i].x + 50, points[i].y };
        control[i].end = (rlVector2){ points[i + 1].x - 50, points[i + 1].y };
    }

    // Spline config variables
    float splineThickness = 8.0f;
    int splineTypeActive = SPLINE_LINEAR; // 0-Linear, 1-BSpline, 2-CatmullRom, 3-Bezier
    bool splineTypeEditMode = false; 
    bool splineHelpersActive = true;
    
    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Spline points creation logic (at the end of spline)
        if (rlIsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && (pointCount < MAX_SPLINE_POINTS))
        {
            points[pointCount] = rlGetMousePosition();
            int i = pointCount - 1;
            control[i].start = (rlVector2){ points[i].x + 50, points[i].y };
            control[i].end = (rlVector2){ points[i + 1].x - 50, points[i + 1].y };
            pointCount++;
        }

        // Spline point focus and selection logic
        for (int i = 0; i < pointCount; i++)
        {
            if (rlCheckCollisionPointCircle(rlGetMousePosition(), points[i], 8.0f))
            {
                focusedPoint = i;
                if (rlIsMouseButtonDown(MOUSE_LEFT_BUTTON)) selectedPoint = i; 
                break;
            }
            else focusedPoint = -1;
        }
        
        // Spline point movement logic
        if (selectedPoint >= 0)
        {
            points[selectedPoint] = rlGetMousePosition();
            if (rlIsMouseButtonReleased(MOUSE_LEFT_BUTTON)) selectedPoint = -1;
        }
        
        // Cubic Bezier spline control points logic
        if ((splineTypeActive == SPLINE_BEZIER) && (focusedPoint == -1))
        {
            // Spline control point focus and selection logic
            for (int i = 0; i < pointCount - 1; i++)
            {
                if (rlCheckCollisionPointCircle(rlGetMousePosition(), control[i].start, 6.0f))
                {
                    focusedControlPoint = &control[i].start;
                    if (rlIsMouseButtonDown(MOUSE_LEFT_BUTTON)) selectedControlPoint = &control[i].start; 
                    break;
                }
                else if (rlCheckCollisionPointCircle(rlGetMousePosition(), control[i].end, 6.0f))
                {
                    focusedControlPoint = &control[i].end;
                    if (rlIsMouseButtonDown(MOUSE_LEFT_BUTTON)) selectedControlPoint = &control[i].end; 
                    break;
                }
                else focusedControlPoint = NULL;
            }
            
            // Spline control point movement logic
            if (selectedControlPoint != NULL)
            {
                *selectedControlPoint = rlGetMousePosition();
                if (rlIsMouseButtonReleased(MOUSE_LEFT_BUTTON)) selectedControlPoint = NULL;
            }
        }
        
        // Spline selection logic
        if (rlIsKeyPressed(KEY_ONE)) splineTypeActive = 0;
        else if (rlIsKeyPressed(KEY_TWO)) splineTypeActive = 1;
        else if (rlIsKeyPressed(KEY_THREE)) splineTypeActive = 2;
        else if (rlIsKeyPressed(KEY_FOUR)) splineTypeActive = 3;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);
        
            if (splineTypeActive == SPLINE_LINEAR)
            {
                // Draw spline: linear
                rlDrawSplineLinear(points, pointCount, splineThickness, RED);
            }
            else if (splineTypeActive == SPLINE_BASIS)
            {
                // Draw spline: basis
                rlDrawSplineBasis(points, pointCount, splineThickness, RED);  // Provide connected points array

                /*
                for (int i = 0; i < (pointCount - 3); i++)
                {
                    // Drawing individual segments, not considering thickness connection compensation
                    rlDrawSplineSegmentBasis(points[i], points[i + 1], points[i + 2], points[i + 3], splineThickness, MAROON);
                }
                */
            }
            else if (splineTypeActive == SPLINE_CATMULLROM)
            {
                // Draw spline: catmull-rom
                rlDrawSplineCatmullRom(points, pointCount, splineThickness, RED); // Provide connected points array
                
                /*
                for (int i = 0; i < (pointCount - 3); i++)
                {
                    // Drawing individual segments, not considering thickness connection compensation
                    rlDrawSplineSegmentCatmullRom(points[i], points[i + 1], points[i + 2], points[i + 3], splineThickness, MAROON);
                }
                */
            }
            else if (splineTypeActive == SPLINE_BEZIER)
            {
                // NOTE: Cubic-bezier spline requires the 2 control points of each segnment to be 
                // provided interleaved with the start and end point of every segment
                for (int i = 0; i < (pointCount - 1); i++) 
                {
                    pointsInterleaved[3*i] = points[i];
                    pointsInterleaved[3*i + 1] = control[i].start;
                    pointsInterleaved[3*i + 2] = control[i].end;
                }
                
                pointsInterleaved[3*(pointCount - 1)] = points[pointCount - 1];

                // Draw spline: cubic-bezier (with control points)
                rlDrawSplineBezierCubic(pointsInterleaved, 3*(pointCount - 1) + 1, splineThickness, RED);
                
                /*
                for (int i = 0; i < 3*(pointCount - 1); i += 3)
                {
                    // Drawing individual segments, not considering thickness connection compensation
                    rlDrawSplineSegmentBezierCubic(pointsInterleaved[i], pointsInterleaved[i + 1], pointsInterleaved[i + 2], pointsInterleaved[i + 3], splineThickness, MAROON);
                }
                */

                // Draw spline control points
                for (int i = 0; i < pointCount - 1; i++)
                {
                    // Every cubic bezier point have two control points
                    rlDrawCircleV(control[i].start, 6, GOLD);
                    rlDrawCircleV(control[i].end, 6, GOLD);
                    if (focusedControlPoint == &control[i].start) rlDrawCircleV(control[i].start, 8, GREEN);
                    else if (focusedControlPoint == &control[i].end) rlDrawCircleV(control[i].end, 8, GREEN);
                    rlDrawLineEx(points[i], control[i].start, 1.0f, LIGHTGRAY);
                    rlDrawLineEx(points[i + 1], control[i].end, 1.0f, LIGHTGRAY);
                
                    // Draw spline control lines
                    rlDrawLineV(points[i], control[i].start, GRAY);
                    //rlDrawLineV(control[i].start, control[i].end, LIGHTGRAY);
                    rlDrawLineV(control[i].end, points[i + 1], GRAY);
                }
            }

            if (splineHelpersActive)
            {
                // Draw spline point helpers
                for (int i = 0; i < pointCount; i++)
                {
                    rlDrawCircleLinesV(points[i], (focusedPoint == i)? 12.0f : 8.0f, (focusedPoint == i)? BLUE: DARKBLUE);
                    if ((splineTypeActive != SPLINE_LINEAR) &&
                        (splineTypeActive != SPLINE_BEZIER) &&
                        (i < pointCount - 1)) rlDrawLineV(points[i], points[i + 1], GRAY);

                    rlDrawText(rlTextFormat("[%.0f, %.0f]", points[i].x, points[i].y), (int)points[i].x, (int)points[i].y + 10, 10, BLACK);
                }
            }

            // Check all possible UI states that require controls lock
            if (splineTypeEditMode) GuiLock();
            
            // Draw spline config
            GuiLabel((rlRectangle){ 12, 62, 140, 24 }, rlTextFormat("Spline thickness: %i", (int)splineThickness));
            GuiSliderBar((rlRectangle){ 12, 60 + 24, 140, 16 }, NULL, NULL, &splineThickness, 1.0f, 40.0f);

            GuiCheckBox((rlRectangle){ 12, 110, 20, 20 }, "Show point helpers", &splineHelpersActive);

            GuiUnlock();

            GuiLabel((rlRectangle){ 12, 10, 140, 24 }, "Spline type:");
            if (GuiDropdownBox((rlRectangle){ 12, 8 + 24, 140, 28 }, "LINEAR;BSPLINE;CATMULLROM;BEZIER", &splineTypeActive, splineTypeEditMode)) splineTypeEditMode = !splineTypeEditMode;

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
