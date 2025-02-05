/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rcamera.h"

#define MAX_COLUMNS 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (rlVector3){ 0.0f, 2.0f, 4.0f };    // Camera position
    camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0 };
    rlVector3 positions[MAX_COLUMNS] = { 0 };
    rlColor colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)rlGetRandomValue(1, 12);
        positions[i] = (rlVector3){ (float)rlGetRandomValue(-15, 15), heights[i]/2.0f, (float)rlGetRandomValue(-15, 15) };
        colors[i] = (rlColor){ rlGetRandomValue(20, 255), rlGetRandomValue(10, 55), 30, 255 };
    }

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Switch camera mode
        if (rlIsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (rlIsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (rlIsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (rlIsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        // Switch camera projection
        if (rlIsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                // Create isometric view
                cameraMode = CAMERA_THIRD_PERSON;
                // Note: The target distance is related to the render distance in the orthographic projection
                camera.position = (rlVector3){ 0.0f, 2.0f, -100.0f };
                camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_ORTHOGRAPHIC;
                camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
                CameraYaw(&camera, -135 * DEG2RAD, true);
                CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
            }
            else if (camera.projection == CAMERA_ORTHOGRAPHIC)
            {
                // Reset to default view
                cameraMode = CAMERA_THIRD_PERSON;
                camera.position = (rlVector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (rlVector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's reecommended to compute camera movement manually
        rlUpdateCamera(&camera, cameraMode);                  // Update camera

/*
        // Camera PRO usage example (EXPERIMENTAL)
        // This new camera function allows custom movement/rotation values to be directly provided
        // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
        rlUpdateCameraPro(&camera,
            (rlVector3){
                (rlIsKeyDown(KEY_W) || rlIsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (rlIsKeyDown(KEY_S) || rlIsKeyDown(KEY_DOWN))*0.1f,    
                (rlIsKeyDown(KEY_D) || rlIsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (rlIsKeyDown(KEY_A) || rlIsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (rlVector3){
                rlGetMouseDelta().x*0.05f,                            // Rotation: yaw
                rlGetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            rlGetMouseWheelMove()*2.0f);                              // Move to target (zoom)
*/
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawPlane((rlVector3){ 0.0f, 0.0f, 0.0f }, (rlVector2){ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
                rlDrawCube((rlVector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                rlDrawCube((rlVector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                rlDrawCube((rlVector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    rlDrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    rlDrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                }

                // Draw player cube
                if (cameraMode == CAMERA_THIRD_PERSON)
                {
                    rlDrawCube(camera.target, 0.5f, 0.5f, 0.5f, PURPLE);
                    rlDrawCubeWires(camera.target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
                }

            rlEndMode3D();

            // Draw info boxes
            rlDrawRectangle(5, 5, 330, 100, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines(5, 5, 330, 100, BLUE);

            rlDrawText("Camera controls:", 15, 15, 10, BLACK);
            rlDrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            rlDrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            rlDrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
            rlDrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
            rlDrawText("- Camera projection key: P", 15, 90, 10, BLACK);

            rlDrawRectangle(600, 5, 195, 100, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines(600, 5, 195, 100, BLUE);

            rlDrawText("Camera status:", 610, 15, 10, BLACK);
            rlDrawText(rlTextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
                                              (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                              (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                              (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
            rlDrawText(rlTextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                                    (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
            rlDrawText(rlTextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            rlDrawText(rlTextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            rlDrawText(rlTextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}