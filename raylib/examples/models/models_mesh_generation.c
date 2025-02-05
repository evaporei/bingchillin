/*******************************************************************************************
*
*   raylib example - procedural mesh generation
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

#define NUM_MODELS  9               // Parametric 3d shapes to generate

static rlMesh GenMeshCustom(void);    // Generate a simple triangle mesh from code

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");

    // We generate a checked image for texturing
    rlImage checked = rlGenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = rlLoadTextureFromImage(checked);
    rlUnloadImage(checked);

    rlModel models[NUM_MODELS] = { 0 };

    models[0] = rlLoadModelFromMesh(rlGenMeshPlane(2, 2, 4, 3));
    models[1] = rlLoadModelFromMesh(rlGenMeshCube(2.0f, 1.0f, 2.0f));
    models[2] = rlLoadModelFromMesh(rlGenMeshSphere(2, 32, 32));
    models[3] = rlLoadModelFromMesh(rlGenMeshHemiSphere(2, 16, 16));
    models[4] = rlLoadModelFromMesh(rlGenMeshCylinder(1, 2, 16));
    models[5] = rlLoadModelFromMesh(rlGenMeshTorus(0.25f, 4.0f, 16, 32));
    models[6] = rlLoadModelFromMesh(rlGenMeshKnot(1.0f, 2.0f, 16, 128));
    models[7] = rlLoadModelFromMesh(rlGenMeshPoly(5, 2.0f));
    models[8] = rlLoadModelFromMesh(GenMeshCustom());
    
    // Generated meshes could be exported as .obj files
    //rlExportMesh(models[0].meshes[0], "plane.obj");
    //rlExportMesh(models[1].meshes[0], "cube.obj");
    //rlExportMesh(models[2].meshes[0], "sphere.obj");
    //rlExportMesh(models[3].meshes[0], "hemisphere.obj");
    //rlExportMesh(models[4].meshes[0], "cylinder.obj");
    //rlExportMesh(models[5].meshes[0], "torus.obj");
    //rlExportMesh(models[6].meshes[0], "knot.obj");
    //rlExportMesh(models[7].meshes[0], "poly.obj");
    //rlExportMesh(models[8].meshes[0], "custom.obj");

    // Set checked texture as default diffuse component for all models material
    for (int i = 0; i < NUM_MODELS; i++) models[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // rlModel drawing position
    rlVector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;

    rlSetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, CAMERA_ORBITAL);

        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            currentModel = (currentModel + 1)%NUM_MODELS; // Cycle between the textures
        }

        if (rlIsKeyPressed(KEY_RIGHT))
        {
            currentModel++;
            if (currentModel >= NUM_MODELS) currentModel = 0;
        }
        else if (rlIsKeyPressed(KEY_LEFT))
        {
            currentModel--;
            if (currentModel < 0) currentModel = NUM_MODELS - 1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);

               rlDrawModel(models[currentModel], position, 1.0f, WHITE);
               rlDrawGrid(10, 1.0);

            rlEndMode3D();

            rlDrawRectangle(30, 400, 310, 30, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines(30, 400, 310, 30, rlFade(DARKBLUE, 0.5f));
            rlDrawText("MOUSE LEFT BUTTON to CYCLE PROCEDURAL MODELS", 40, 410, 10, BLUE);

            switch(currentModel)
            {
                case 0: rlDrawText("PLANE", 680, 10, 20, DARKBLUE); break;
                case 1: rlDrawText("CUBE", 680, 10, 20, DARKBLUE); break;
                case 2: rlDrawText("SPHERE", 680, 10, 20, DARKBLUE); break;
                case 3: rlDrawText("HEMISPHERE", 640, 10, 20, DARKBLUE); break;
                case 4: rlDrawText("CYLINDER", 680, 10, 20, DARKBLUE); break;
                case 5: rlDrawText("TORUS", 680, 10, 20, DARKBLUE); break;
                case 6: rlDrawText("KNOT", 680, 10, 20, DARKBLUE); break;
                case 7: rlDrawText("POLY", 680, 10, 20, DARKBLUE); break;
                case 8: rlDrawText("Custom (triangle)", 580, 10, 20, DARKBLUE); break;
                default: break;
            }

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    for (int i = 0; i < NUM_MODELS; i++) rlUnloadModel(models[i]);

    rlCloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Generate a simple triangle mesh from code
static rlMesh GenMeshCustom(void)
{
    rlMesh mesh = { 0 };
    mesh.triangleCount = 1;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)rlMemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)rlMemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)rlMemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (1, 0, 2)
    mesh.vertices[3] = 1;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = 2;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.5f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (2, 0, 0)
    mesh.vertices[6] = 2;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] =0;

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    rlUploadMesh(&mesh, false);

    return mesh;
}
