/*******************************************************************************************
*
*   raylib [core] example - Generates a random sequence
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.0
*
*   Example contributed by Dalton Overmyer (@REDl3east) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Dalton Overmyer (@REDl3east)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h> // Required for: malloc() and free()

typedef struct ColorRect{
  rlColor c;
  rlRectangle r;
} ColorRect;

static rlColor GenerateRandomColor();
static ColorRect* GenerateRandomColorRectSequence(float rectCount, float rectWidth, float screenWidth, float screenHeight);
static void ShuffleColorRectSequence(ColorRect* rectangles, int rectCount);
static void DrawTextCenterKeyHelp(const char* key, const char* text, int posX, int posY, int fontSize, rlColor color);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  rlInitWindow(screenWidth, screenHeight, "raylib [core] example - Generates a random sequence");

  int rectCount = 20;
  float rectSize = (float)screenWidth/rectCount;
  ColorRect* rectangles = GenerateRandomColorRectSequence((float)rectCount, rectSize, (float)screenWidth, 0.75f * screenHeight);

  rlSetTargetFPS(60);
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!rlWindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------

    if(rlIsKeyPressed(KEY_SPACE))
    {
      ShuffleColorRectSequence(rectangles, rectCount);
    }

    if(rlIsKeyPressed(KEY_UP))
    {
      rectCount++;
      rectSize = (float)screenWidth/rectCount;
      free(rectangles);
      rectangles = GenerateRandomColorRectSequence((float)rectCount, rectSize, (float)screenWidth, 0.75f * screenHeight);
    }

    if(rlIsKeyPressed(KEY_DOWN))
    {
      if(rectCount >= 4){
        rectCount--;
        rectSize = (float)screenWidth/rectCount;
        free(rectangles);
        rectangles = GenerateRandomColorRectSequence((float)rectCount, rectSize, (float)screenWidth, 0.75f * screenHeight);
      }
    }

    // Draw
    //----------------------------------------------------------------------------------
    rlBeginDrawing();

    rlClearBackground(RAYWHITE);
    
    int fontSize = 20;
    for(int x=0;x<rectCount;x++)
    {
      rlDrawRectangleRec(rectangles[x].r, rectangles[x].c);
      DrawTextCenterKeyHelp("SPACE", "to shuffle the sequence.", 10, screenHeight - 96, fontSize, BLACK);
      DrawTextCenterKeyHelp("UP", "to add a rectangle and generate a new sequence.", 10, screenHeight - 64, fontSize, BLACK);
      DrawTextCenterKeyHelp("DOWN", "to remove a rectangle and generate a new sequence.", 10, screenHeight - 32, fontSize, BLACK);
    }

    const char* rectCountText = rlTextFormat("%d rectangles", rectCount);
    int rectCountTextSize = rlMeasureText(rectCountText, fontSize);
    rlDrawText(rectCountText, screenWidth - rectCountTextSize - 10, 10, fontSize, BLACK);

    rlDrawFPS(10, 10);

    rlEndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------

  free(rectangles);
  rlCloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static rlColor GenerateRandomColor()
{
  return CLITERAL(rlColor){
    rlGetRandomValue(0, 255),
    rlGetRandomValue(0, 255),
    rlGetRandomValue(0, 255),
    255,
  };
}

static ColorRect* GenerateRandomColorRectSequence(float rectCount, float rectWidth, float screenWidth, float screenHeight){
  int *seq = rlLoadRandomSequence((unsigned int)rectCount, 0, (unsigned int)rectCount-1);
  ColorRect* rectangles = (ColorRect *)malloc((int)rectCount*sizeof(ColorRect));

  float rectSeqWidth = rectCount * rectWidth;
  float startX = (screenWidth - rectSeqWidth) * 0.5f;

  for(int x=0;x<rectCount;x++){
    int rectHeight = (int)Remap((float)seq[x], 0, rectCount-1, 0, screenHeight);
    rectangles[x].c = GenerateRandomColor();
    rectangles[x].r = CLITERAL(rlRectangle){
      startX + x * rectWidth, screenHeight - rectHeight, rectWidth, (float)rectHeight
    };
  }
  rlUnloadRandomSequence(seq);
  return rectangles;
}

static void ShuffleColorRectSequence(ColorRect* rectangles, int rectCount)
{
  int *seq = rlLoadRandomSequence(rectCount, 0, rectCount-1);
  for(int i1=0;i1<rectCount;i1++){
    ColorRect* r1 = &rectangles[i1];
    ColorRect* r2 = &rectangles[seq[i1]];

    // swap only the color and height
    ColorRect tmp = *r1;
    r1->c = r2->c;
    r1->r.height = r2->r.height;
    r1->r.y = r2->r.y;
    r2->c = tmp.c;
    r2->r.height = tmp.r.height;
    r2->r.y = tmp.r.y;

  }
  rlUnloadRandomSequence(seq);
}

static void DrawTextCenterKeyHelp(const char* key, const char* text, int posX, int posY, int fontSize, rlColor color)
{
  int spaceSize = rlMeasureText(" ", fontSize); 
  int pressSize = rlMeasureText("Press", fontSize); 
  int keySize = rlMeasureText(key, fontSize); 
  int textSize = rlMeasureText(text, fontSize); 
  int totalSize = pressSize + 2 * spaceSize + keySize + 2 * spaceSize + textSize;
  int textSizeCurrent = 0;

  rlDrawText("Press", posX, posY, fontSize, color);
  textSizeCurrent += pressSize + 2 * spaceSize;
  rlDrawText(key, posX + textSizeCurrent, posY, fontSize, RED);
  rlDrawRectangle(posX + textSizeCurrent, posY + fontSize, keySize, 3, RED);
  textSizeCurrent += keySize + 2 * spaceSize;
  rlDrawText(text, posX + textSizeCurrent, posY, fontSize, color);
}