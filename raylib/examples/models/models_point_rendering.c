/*******************************************************************************************
*
*   raylib example - point rendering
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.0
*
*   Example contributed by Reese Gallagher (@satchelfrost) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2024 Reese Gallagher (@satchelfrost)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>             // Required for: rand()
#include <math.h>               // Required for: cos(), sin()

#define MAX_POINTS 10000000     // 10 million
#define MIN_POINTS 1000         // 1 thousand

// Generate mesh using points
rlMesh GenMeshPoints(int numPoints);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - point rendering");

    Camera camera = {
        .position   = { 3.0f, 3.0f, 3.0f },
        .target     = { 0.0f, 0.0f, 0.0f },
        .up         = { 0.0f, 1.0f, 0.0f },
        .fovy       = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    rlVector3 position = { 0.0f, 0.0f, 0.0f };
    bool useDrawModelPoints = true;
    bool numPointsChanged = false;
    int numPoints = 1000;
    
    rlMesh mesh = GenMeshPoints(numPoints);
    rlModel model = rlLoadModelFromMesh(mesh);
    
    //rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while(!rlWindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        if (rlIsKeyPressed(KEY_SPACE)) useDrawModelPoints = !useDrawModelPoints;
        if (rlIsKeyPressed(KEY_UP))
        {
            numPoints = (numPoints*10 > MAX_POINTS)? MAX_POINTS : numPoints*10;
            numPointsChanged = true;
        }
        if (rlIsKeyPressed(KEY_DOWN))
        {
            numPoints = (numPoints/10 < MIN_POINTS)? MIN_POINTS : numPoints/10;
            numPointsChanged = true;
        }

        // Upload a different point cloud size
        if (numPointsChanged)
        {
            rlUnloadModel(model);
            mesh = GenMeshPoints(numPoints);
            model = rlLoadModelFromMesh(mesh);
            numPointsChanged = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();
            rlClearBackground(BLACK);

            rlBeginMode3D(camera);

                // The new method only uploads the points once to the GPU
                if (useDrawModelPoints)
                {
                    rlDrawModelPoints(model, position, 1.0f, WHITE);
                }
                else
                {
                    // The old method must continually draw the "points" (lines)
                    for (int i = 0; i < numPoints; i++)
                    {
                        rlVector3 pos = {
                            .x = mesh.vertices[i*3 + 0],
                            .y = mesh.vertices[i*3 + 1],
                            .z = mesh.vertices[i*3 + 2],
                        };
                        rlColor color = {
                            .r = mesh.colors[i*4 + 0],
                            .g = mesh.colors[i*4 + 1],
                            .b = mesh.colors[i*4 + 2],
                            .a = mesh.colors[i*4 + 3],
                        };
                        
                        rlDrawPoint3D(pos, color);
                    }
                }

                // Draw a unit sphere for reference
                rlDrawSphereWires(position, 1.0f, 10, 10, YELLOW);
                
            rlEndMode3D();

            // Draw UI text
            rlDrawText(rlTextFormat("Point Count: %d", numPoints), 20, screenHeight - 50, 40, WHITE);
            rlDrawText("Up - increase points", 20, 70, 20, WHITE);
            rlDrawText("Down - decrease points", 20, 100, 20, WHITE);
            rlDrawText("Space - drawing function", 20, 130, 20, WHITE);
            
            if (useDrawModelPoints) rlDrawText("Using: rlDrawModelPoints()", 20, 160, 20, GREEN);
            else rlDrawText("Using: rlDrawPoint3D()", 20, 160, 20, RED);
            
            rlDrawFPS(10, 10);
            
        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(model);

    rlCloseWindow();
    //--------------------------------------------------------------------------------------
    return 0;
}

// Generate a spherical point cloud
rlMesh GenMeshPoints(int numPoints)
{
    rlMesh mesh = { 
        .triangleCount = 1,
        .vertexCount = numPoints,
        .vertices = (float *)rlMemAlloc(numPoints*3*sizeof(float)),
        .colors = (unsigned char*)rlMemAlloc(numPoints*4*sizeof(unsigned char)),
    };

    // https://en.wikipedia.org/wiki/Spherical_coordinate_system
    for (int i = 0; i < numPoints; i++)
    {
        float theta = PI*rand()/RAND_MAX;
        float phi = 2.0f*PI*rand()/RAND_MAX;
        float r = 10.0f*rand()/RAND_MAX;
        
        mesh.vertices[i*3 + 0] = r*sin(theta)*cos(phi);
        mesh.vertices[i*3 + 1] = r*sin(theta)*sin(phi);
        mesh.vertices[i*3 + 2] = r*cos(theta);
        
        rlColor color = rlColorFromHSV(r*360.0f, 1.0f, 1.0f);
        
        mesh.colors[i*4 + 0] = color.r;
        mesh.colors[i*4 + 1] = color.g;
        mesh.colors[i*4 + 2] = color.b;
        mesh.colors[i*4 + 3] = color.a;
    }

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    rlUploadMesh(&mesh, false);
    
    return mesh;
}
