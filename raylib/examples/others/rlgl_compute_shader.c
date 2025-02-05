/*******************************************************************************************
*
*   raylib [rlgl] example - compute shader - Conway's Game of Life
*
*   NOTE: This example requires raylib OpenGL 4.3 versions for compute shaders support,
*         shaders used in this example are #version 430 (OpenGL 4.3)
*
*   Example originally created with raylib 4.0, last time updated with raylib 2.5
*
*   Example contributed by Teddy Astie (@tsnake41) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Teddy Astie (@tsnake41)
*
********************************************************************************************/

#include "raylib.h"
#include "rlgl.h"

#include <stdlib.h>

// IMPORTANT: This must match gol*.glsl GOL_WIDTH constant.
// This must be a multiple of 16 (check golLogic compute dispatch).
#define GOL_WIDTH 768

// Maximum amount of queued draw commands (squares draw from mouse down events).
#define MAX_BUFFERED_TRANSFERTS 48

// Game Of Life Update Command
typedef struct GolUpdateCmd {
    unsigned int x;         // x coordinate of the gol command
    unsigned int y;         // y coordinate of the gol command
    unsigned int w;         // width of the filled zone
    unsigned int enabled;   // whether to enable or disable zone
} GolUpdateCmd;

// Game Of Life Update Commands SSBO
typedef struct GolUpdateSSBO {
    unsigned int count;
    GolUpdateCmd commands[MAX_BUFFERED_TRANSFERTS];
} GolUpdateSSBO;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    rlInitWindow(GOL_WIDTH, GOL_WIDTH, "raylib [rlgl] example - compute shader - game of life");

    const rlVector2 resolution = { GOL_WIDTH, GOL_WIDTH };
    unsigned int brushSize = 8;

    // Game of Life logic compute shader
    char *golLogicCode = rlLoadFileText("resources/shaders/glsl430/gol.glsl");
    unsigned int golLogicShader = rlCompileShader(golLogicCode, RL_COMPUTE_SHADER);
    unsigned int golLogicProgram = rlLoadComputeShaderProgram(golLogicShader);
    rlUnloadFileText(golLogicCode);

    // Game of Life logic render shader
    rlShader golRenderShader = rlLoadShader(NULL, "resources/shaders/glsl430/gol_render.glsl");
    int resUniformLoc = rlGetShaderLocation(golRenderShader, "resolution");

    // Game of Life transfert shader (CPU<->GPU download and upload)
    char *golTransfertCode = rlLoadFileText("resources/shaders/glsl430/gol_transfert.glsl");
    unsigned int golTransfertShader = rlCompileShader(golTransfertCode, RL_COMPUTE_SHADER);
    unsigned int golTransfertProgram = rlLoadComputeShaderProgram(golTransfertShader);
    rlUnloadFileText(golTransfertCode);

    // Load shader storage buffer object (SSBO), id returned
    unsigned int ssboA = rlLoadShaderBuffer(GOL_WIDTH*GOL_WIDTH*sizeof(unsigned int), NULL, RL_DYNAMIC_COPY);
    unsigned int ssboB = rlLoadShaderBuffer(GOL_WIDTH*GOL_WIDTH*sizeof(unsigned int), NULL, RL_DYNAMIC_COPY);
    unsigned int ssboTransfert = rlLoadShaderBuffer(sizeof(GolUpdateSSBO), NULL, RL_DYNAMIC_COPY);

    GolUpdateSSBO transfertBuffer = { 0 };

    // Create a white texture of the size of the window to update
    // each pixel of the window using the fragment shader: golRenderShader
    rlImage whiteImage = rlGenImageColor(GOL_WIDTH, GOL_WIDTH, WHITE);
    rlTexture whiteTex = rlLoadTextureFromImage(whiteImage);
    rlUnloadImage(whiteImage);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        brushSize += (int)rlGetMouseWheelMove();

        if ((rlIsMouseButtonDown(MOUSE_BUTTON_LEFT) || rlIsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            && (transfertBuffer.count < MAX_BUFFERED_TRANSFERTS))
        {
            // Buffer a new command
            transfertBuffer.commands[transfertBuffer.count].x = rlGetMouseX() - brushSize/2;
            transfertBuffer.commands[transfertBuffer.count].y = rlGetMouseY() - brushSize/2;
            transfertBuffer.commands[transfertBuffer.count].w = brushSize;
            transfertBuffer.commands[transfertBuffer.count].enabled = rlIsMouseButtonDown(MOUSE_BUTTON_LEFT);
            transfertBuffer.count++;
        }
        else if (transfertBuffer.count > 0)  // Process transfert buffer
        {
            // Send SSBO buffer to GPU
            rlUpdateShaderBuffer(ssboTransfert, &transfertBuffer, sizeof(GolUpdateSSBO), 0);

            // Process SSBO commands on GPU
            rlEnableShader(golTransfertProgram);
            rlBindShaderBuffer(ssboA, 1);
            rlBindShaderBuffer(ssboTransfert, 3);
            rlComputeShaderDispatch(transfertBuffer.count, 1, 1); // Each GPU unit will process a command!
            rlDisableShader();

            transfertBuffer.count = 0;
        }
        else
        {
            // Process game of life logic
            rlEnableShader(golLogicProgram);
            rlBindShaderBuffer(ssboA, 1);
            rlBindShaderBuffer(ssboB, 2);
            rlComputeShaderDispatch(GOL_WIDTH/16, GOL_WIDTH/16, 1);
            rlDisableShader();

            // ssboA <-> ssboB
            int temp = ssboA;
            ssboA = ssboB;
            ssboB = temp;
        }

        rlBindShaderBuffer(ssboA, 1);
        rlSetShaderValue(golRenderShader, resUniformLoc, &resolution, SHADER_UNIFORM_VEC2);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(BLANK);

            rlBeginShaderMode(golRenderShader);
                rlDrawTexture(whiteTex, 0, 0, WHITE);
            rlEndShaderMode();

            rlDrawRectangleLines(rlGetMouseX() - brushSize/2, rlGetMouseY() - brushSize/2, brushSize, brushSize, RED);

            rlDrawText("Use Mouse wheel to increase/decrease brush size", 10, 10, 20, WHITE);
            rlDrawFPS(rlGetScreenWidth() - 100, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload shader buffers objects.
    rlUnloadShaderBuffer(ssboA);
    rlUnloadShaderBuffer(ssboB);
    rlUnloadShaderBuffer(ssboTransfert);

    // Unload compute shader programs
    rlUnloadShaderProgram(golTransfertProgram);
    rlUnloadShaderProgram(golLogicProgram);

    UnloadTexture(whiteTex);            // Unload white texture
    rlUnloadShader(golRenderShader);      // Unload rendering fragment shader

    rlCloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
