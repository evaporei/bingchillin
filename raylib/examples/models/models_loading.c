/*******************************************************************************************
*
*   raylib [models] example - Models loading
*
*   NOTE: raylib supports multiple models file formats:
*
*     - OBJ  > Text file format. Must include vertex position-texcoords-normals information,
*              if files references some .mtl materials file, it will be loaded (or try to).
*     - GLTF > Text/binary file format. Includes lot of information and it could
*              also reference external files, raylib will try loading mesh and materials data.
*     - IQM  > Binary file format. Includes mesh vertex data but also animation data,
*              raylib can load .iqm animations.
*     - VOX  > Binary file format. MagikaVoxel mesh format:
*              https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*     - M3D  > Binary file format. rlModel 3D format:
*              https://bztsrc.gitlab.io/model3d
*
*   Example originally created with raylib 2.0, last time updated with raylib 4.2
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - models loading");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (rlVector3){ 50.0f, 50.0f, 50.0f }; // Camera position
    camera.target = (rlVector3){ 0.0f, 10.0f, 0.0f };     // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    rlModel model = rlLoadModel("resources/models/obj/castle.obj");                 // Load model
    Texture2D texture = LoadTexture("resources/models/obj/castle_diffuse.png"); // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    rlVector3 position = { 0.0f, 0.0f, 0.0f };                    // Set model position

    rlBoundingBox bounds = rlGetMeshBoundingBox(model.meshes[0]);   // Set model bounds

    // NOTE: bounds are calculated from the original size of the model,
    // if model is scaled on drawing, bounds must be also scaled

    bool selected = false;          // Selected object flag

    rlDisableCursor();                // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Load new models/textures on drag&drop
        if (rlIsFileDropped())
        {
            rlFilePathList droppedFiles = rlLoadDroppedFiles();

            if (droppedFiles.count == 1) // Only support one file dropped
            {
                if (rlIsFileExtension(droppedFiles.paths[0], ".obj") ||
                    rlIsFileExtension(droppedFiles.paths[0], ".gltf") ||
                    rlIsFileExtension(droppedFiles.paths[0], ".glb") ||
                    rlIsFileExtension(droppedFiles.paths[0], ".vox") ||
                    rlIsFileExtension(droppedFiles.paths[0], ".iqm") ||
                    rlIsFileExtension(droppedFiles.paths[0], ".m3d"))       // rlModel file formats supported
                {
                    rlUnloadModel(model);                         // Unload previous model
                    model = rlLoadModel(droppedFiles.paths[0]);   // Load new model
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set current map diffuse texture

                    bounds = rlGetMeshBoundingBox(model.meshes[0]);

                    // TODO: Move camera position from target enough distance to visualize model properly
                }
                else if (rlIsFileExtension(droppedFiles.paths[0], ".png"))  // rlTexture file formats supported
                {
                    // Unload current model texture and load new one
                    UnloadTexture(texture);
                    texture = LoadTexture(droppedFiles.paths[0]);
                    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                }
            }

            rlUnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }

        // Select model on mouse click
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Check collision between ray and box
            if (rlGetRayCollisionBox(rlGetScreenToWorldRay(rlGetMousePosition(), camera), bounds).hit) selected = !selected;
            else selected = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

                rlDrawModel(model, position, 1.0f, WHITE);        // Draw 3d model with texture

                rlDrawGrid(20, 10.0f);         // Draw a grid

                if (selected) rlDrawBoundingBox(bounds, GREEN);   // Draw selection box

            rlEndMode3D();

            rlDrawText("Drag & drop model to load mesh/texture.", 10, rlGetScreenHeight() - 20, 10, DARKGRAY);
            if (selected) rlDrawText("MODEL SELECTED", rlGetScreenWidth() - 110, 10, 10, GREEN);

            rlDrawText("(c) Castle 3D model by Alberto Cano", screenWidth - 200, screenHeight - 20, 10, GRAY);

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);     // Unload texture
    rlUnloadModel(model);         // Unload model

    rlCloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}