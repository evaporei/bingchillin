/*******************************************************************************************
*
*   raylib [models] example - rlMesh picking in 3d mode, ground plane, triangle, mesh
*
*   Example originally created with raylib 1.7, last time updated with raylib 4.0
*
*   Example contributed by Joel Davis (@joeld42) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2024 Joel Davis (@joeld42) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#define FLT_MAX     340282346638528859811704183484516925440.0f     // Maximum value of a float, from bit pattern 01111111011111111111111111111111

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - mesh picking");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 20.0f, 20.0f, 20.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 8.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.6f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    rlRay ray = { 0 };        // Picking ray

    rlModel tower = rlLoadModel("resources/models/obj/turret.obj");                 // Load OBJ model
    Texture2D texture = LoadTexture("resources/models/obj/turret_diffuse.png"); // Load model texture
    tower.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set model diffuse texture

    rlVector3 towerPos = { 0.0f, 0.0f, 0.0f };                        // Set model position
    rlBoundingBox towerBBox = rlGetMeshBoundingBox(tower.meshes[0]);    // Get mesh bounding box

    // Ground quad
    rlVector3 g0 = (rlVector3){ -50.0f, 0.0f, -50.0f };
    rlVector3 g1 = (rlVector3){ -50.0f, 0.0f,  50.0f };
    rlVector3 g2 = (rlVector3){  50.0f, 0.0f,  50.0f };
    rlVector3 g3 = (rlVector3){  50.0f, 0.0f, -50.0f };

    // Test triangle
    rlVector3 ta = (rlVector3){ -25.0f, 0.5f, 0.0f };
    rlVector3 tb = (rlVector3){ -4.0f, 2.5f, 1.0f };
    rlVector3 tc = (rlVector3){ -8.0f, 6.5f, 0.0f };

    rlVector3 bary = { 0.0f, 0.0f, 0.0f };

    // Test sphere
    rlVector3 sp = (rlVector3){ -30.0f, 5.0f, 5.0f };
    float sr = 4.0f;

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (rlIsCursorHidden()) rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);          // Update camera

        // Toggle camera controls
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (rlIsCursorHidden()) rlEnableCursor();
            else rlDisableCursor();
        }

        // Display information about closest hit
        rlRayCollision collision = { 0 };
        char *hitObjectName = "None";
        collision.distance = FLT_MAX;
        collision.hit = false;
        rlColor cursorColor = WHITE;

        // Get ray and test against objects
        ray = rlGetScreenToWorldRay(rlGetMousePosition(), camera);

        // Check ray collision against ground quad
        rlRayCollision groundHitInfo = rlGetRayCollisionQuad(ray, g0, g1, g2, g3);

        if ((groundHitInfo.hit) && (groundHitInfo.distance < collision.distance))
        {
            collision = groundHitInfo;
            cursorColor = GREEN;
            hitObjectName = "Ground";
        }

        // Check ray collision against test triangle
        rlRayCollision triHitInfo = rlGetRayCollisionTriangle(ray, ta, tb, tc);

        if ((triHitInfo.hit) && (triHitInfo.distance < collision.distance))
        {
            collision = triHitInfo;
            cursorColor = PURPLE;
            hitObjectName = "Triangle";

            bary = Vector3Barycenter(collision.point, ta, tb, tc);
        }

        // Check ray collision against test sphere
        rlRayCollision sphereHitInfo = rlGetRayCollisionSphere(ray, sp, sr);

        if ((sphereHitInfo.hit) && (sphereHitInfo.distance < collision.distance))
        {
            collision = sphereHitInfo;
            cursorColor = ORANGE;
            hitObjectName = "Sphere";
        }

        // Check ray collision against bounding box first, before trying the full ray-mesh test
        rlRayCollision boxHitInfo = rlGetRayCollisionBox(ray, towerBBox);

        if ((boxHitInfo.hit) && (boxHitInfo.distance < collision.distance))
        {
            collision = boxHitInfo;
            cursorColor = ORANGE;
            hitObjectName = "Box";

            // Check ray collision against model meshes
            rlRayCollision meshHitInfo = { 0 };
            for (int m = 0; m < tower.meshCount; m++)
            {
                // NOTE: We consider the model.transform for the collision check but 
                // it can be checked against any transform rlMatrix, used when checking against same
                // model drawn multiple times with multiple transforms
                meshHitInfo = rlGetRayCollisionMesh(ray, tower.meshes[m], tower.transform);
                if (meshHitInfo.hit)
                {
                    // Save the closest hit mesh
                    if ((!collision.hit) || (collision.distance > meshHitInfo.distance)) collision = meshHitInfo;
                    
                    break;  // Stop once one mesh collision is detected, the colliding mesh is m
                }
            }

            if (meshHitInfo.hit)
            {
                collision = meshHitInfo;
                cursorColor = ORANGE;
                hitObjectName = "rlMesh";
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                // Draw the tower
                // WARNING: If scale is different than 1.0f,
                // not considered by GetRayCollisionModel()
                rlDrawModel(tower, towerPos, 1.0f, WHITE);

                // Draw the test triangle
                rlDrawLine3D(ta, tb, PURPLE);
                rlDrawLine3D(tb, tc, PURPLE);
                rlDrawLine3D(tc, ta, PURPLE);

                // Draw the test sphere
                rlDrawSphereWires(sp, sr, 8, 8, PURPLE);

                // Draw the mesh bbox if we hit it
                if (boxHitInfo.hit) rlDrawBoundingBox(towerBBox, LIME);

                // If we hit something, draw the cursor at the hit point
                if (collision.hit)
                {
                    rlDrawCube(collision.point, 0.3f, 0.3f, 0.3f, cursorColor);
                    rlDrawCubeWires(collision.point, 0.3f, 0.3f, 0.3f, RED);

                    rlVector3 normalEnd;
                    normalEnd.x = collision.point.x + collision.normal.x;
                    normalEnd.y = collision.point.y + collision.normal.y;
                    normalEnd.z = collision.point.z + collision.normal.z;

                    rlDrawLine3D(collision.point, normalEnd, RED);
                }

                rlDrawRay(ray, MAROON);

                rlDrawGrid(10, 10.0f);

            rlEndMode3D();

            // Draw some debug GUI text
            rlDrawText(rlTextFormat("Hit Object: %s", hitObjectName), 10, 50, 10, BLACK);

            if (collision.hit)
            {
                int ypos = 70;

                rlDrawText(rlTextFormat("Distance: %3.2f", collision.distance), 10, ypos, 10, BLACK);

                rlDrawText(rlTextFormat("Hit Pos: %3.2f %3.2f %3.2f",
                                    collision.point.x,
                                    collision.point.y,
                                    collision.point.z), 10, ypos + 15, 10, BLACK);

                rlDrawText(rlTextFormat("Hit Norm: %3.2f %3.2f %3.2f",
                                    collision.normal.x,
                                    collision.normal.y,
                                    collision.normal.z), 10, ypos + 30, 10, BLACK);

                if (triHitInfo.hit && rlTextIsEqual(hitObjectName, "Triangle"))
                    rlDrawText(rlTextFormat("Barycenter: %3.2f %3.2f %3.2f",  bary.x, bary.y, bary.z), 10, ypos + 45, 10, BLACK);
            }

            rlDrawText("Right click mouse to toggle camera controls", 10, 430, 10, GRAY);

            rlDrawText("(c) Turret 3D model by Alberto Cano", screenWidth - 200, screenHeight - 20, 10, GRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadModel(tower);         // Unload model
    UnloadTexture(texture);     // Unload texture

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}