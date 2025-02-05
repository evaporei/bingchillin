/*******************************************************************************************
*
*   raylib [text] example - Draw 3d
*
*   NOTE: Draw a 2D text in 3D space, each letter is drawn in a quad (or 2 quads if backface is set)
*   where the texture coodinates of each quad map to the texture coordinates of the glyphs
*   inside the font texture.
*
*   A more efficient approach, i believe, would be to render the text in a render texture and
*   map that texture to a plane and render that, or maybe a shader but my method allows more
*   flexibility...for example to change position of each letter individually to make somethink
*   like a wavy text effect.
*    
*   Special thanks to:
*        @Nighten for the DrawTextStyle() code https://github.com/NightenDushi/Raylib_DrawTextStyle
*        Chris Camacho (codifies - http://bedroomcoders.co.uk/) for the alpha discard shader
*
*   Example originally created with raylib 3.5, last time updated with raylib 4.0
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2021-2024 Vlad Adrian (@demizdor)
*
********************************************************************************************/

#include "raylib.h"
#include "rlgl.h"

#include <stddef.h>     // Required for: NULL
#include <math.h>       // Required for: sinf()

// To make it work with the older RLGL module just comment the line below
#define RAYLIB_NEW_RLGL

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
#define LETTER_BOUNDRY_SIZE     0.25f
#define TEXT_MAX_LAYERS         32
#define LETTER_BOUNDRY_COLOR    VIOLET

bool SHOW_LETTER_BOUNDRY = false;
bool SHOW_TEXT_BOUNDRY = false;

//--------------------------------------------------------------------------------------
// Data Types definition
//--------------------------------------------------------------------------------------

// Configuration structure for waving the text
typedef struct WaveTextConfig {
    rlVector3 waveRange;
    rlVector3 waveSpeed;
    rlVector3 waveOffset;
} WaveTextConfig;

//--------------------------------------------------------------------------------------
// Module Functions Declaration
//--------------------------------------------------------------------------------------
// Draw a codepoint in 3D space
static void DrawTextCodepoint3D(rlFont font, int codepoint, rlVector3 position, float fontSize, bool backface, rlColor tint);
// Draw a 2D text in 3D space
static void DrawText3D(rlFont font, const char *text, rlVector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, rlColor tint);
// Measure a text in 3D. For some reason `rlMeasureTextEx()` just doesn't seem to work so i had to use this instead.
static rlVector3 MeasureText3D(rlFont font, const char *text, float fontSize, float fontSpacing, float lineSpacing);

// Draw a 2D text in 3D space and wave the parts that start with `~~` and end with `~~`.
// This is a modified version of the original code by @Nighten found here https://github.com/NightenDushi/Raylib_DrawTextStyle
static void DrawTextWave3D(rlFont font, const char *text, rlVector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, WaveTextConfig *config, float time, rlColor tint);
// Measure a text in 3D ignoring the `~~` chars.
static rlVector3 MeasureTextWave3D(rlFont font, const char *text, float fontSize, float fontSpacing, float lineSpacing);
// Generates a nice color with a random hue
static rlColor GenerateRandomColor(float s, float v);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlSetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_VSYNC_HINT);
    rlInitWindow(screenWidth, screenHeight, "raylib [text] example - draw 2D text in 3D");

    bool spin = true;        // Spin the camera?
    bool multicolor = false; // Multicolor mode

    // Define the camera to look into our 3d world
    rlCamera3D camera = { 0 };
    camera.position = (rlVector3){ -10.0f, 15.0f, -10.0f };   // Camera position
    camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    int camera_mode = CAMERA_ORBITAL;

    rlVector3 cubePosition = { 0.0f, 1.0f, 0.0f };
    rlVector3 cubeSize = { 2.0f, 2.0f, 2.0f };

    // Use the default font
    rlFont font = rlGetFontDefault();
    float fontSize = 8.0f;
    float fontSpacing = 0.5f;
    float lineSpacing = -1.0f;

    // Set the text (using markdown!)
    char text[64] = "Hello ~~World~~ in 3D!";
    rlVector3 tbox = {0};
    int layers = 1;
    int quads = 0;
    float layerDistance = 0.01f;

    WaveTextConfig wcfg;
    wcfg.waveSpeed.x = wcfg.waveSpeed.y = 3.0f; wcfg.waveSpeed.z = 0.5f;
    wcfg.waveOffset.x = wcfg.waveOffset.y = wcfg.waveOffset.z = 0.35f;
    wcfg.waveRange.x = wcfg.waveRange.y = wcfg.waveRange.z = 0.45f;

    float time = 0.0f;

    // Setup a light and dark color
    rlColor light = MAROON;
    rlColor dark = RED;

    // Load the alpha discard shader
    rlShader alphaDiscard = rlLoadShader(NULL, "resources/shaders/glsl330/alpha_discard.fs");

    // Array filled with multiple random colors (when multicolor mode is set)
    rlColor multi[TEXT_MAX_LAYERS] = {0};

    rlDisableCursor();                    // Limit cursor to relative movement inside the window

    rlSetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rlUpdateCamera(&camera, camera_mode);
        
        // Handle font files dropped
        if (rlIsFileDropped())
        {
            rlFilePathList droppedFiles = rlLoadDroppedFiles();

            // NOTE: We only support first ttf file dropped
            if (rlIsFileExtension(droppedFiles.paths[0], ".ttf"))
            {
                rlUnloadFont(font);
                font = rlLoadFontEx(droppedFiles.paths[0], (int)fontSize, 0, 0);
            }
            else if (rlIsFileExtension(droppedFiles.paths[0], ".fnt"))
            {
                rlUnloadFont(font);
                font = rlLoadFont(droppedFiles.paths[0]);
                fontSize = (float)font.baseSize;
            }
            
            rlUnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }

        // Handle Events
        if (rlIsKeyPressed(KEY_F1)) SHOW_LETTER_BOUNDRY = !SHOW_LETTER_BOUNDRY;
        if (rlIsKeyPressed(KEY_F2)) SHOW_TEXT_BOUNDRY = !SHOW_TEXT_BOUNDRY;
        if (rlIsKeyPressed(KEY_F3))
        {
            // Handle camera change
            spin = !spin;
            // we need to reset the camera when changing modes
            camera = (rlCamera3D){ 0 };
            camera.target = (rlVector3){ 0.0f, 0.0f, 0.0f };          // Camera looking at point
            camera.up = (rlVector3){ 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
            camera.fovy = 45.0f;                                    // Camera field-of-view Y
            camera.projection = CAMERA_PERSPECTIVE;                 // Camera mode type

            if (spin)
            {
                camera.position = (rlVector3){ -10.0f, 15.0f, -10.0f };   // Camera position
                camera_mode = CAMERA_ORBITAL;
            }
            else
            {
                camera.position = (rlVector3){ 10.0f, 10.0f, -10.0f };   // Camera position
                camera_mode = CAMERA_FREE;
            }
        }

        // Handle clicking the cube
        if (rlIsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            rlRay ray = rlGetScreenToWorldRay(rlGetMousePosition(), camera);

            // Check collision between ray and box
            rlRayCollision collision = rlGetRayCollisionBox(ray,
                            (rlBoundingBox){(rlVector3){ cubePosition.x - cubeSize.x/2, cubePosition.y - cubeSize.y/2, cubePosition.z - cubeSize.z/2 },
                                          (rlVector3){ cubePosition.x + cubeSize.x/2, cubePosition.y + cubeSize.y/2, cubePosition.z + cubeSize.z/2 }});
            if (collision.hit)
            {
                // Generate new random colors
                light = GenerateRandomColor(0.5f, 0.78f);
                dark = GenerateRandomColor(0.4f, 0.58f);
            }
        }

        // Handle text layers changes
        if (rlIsKeyPressed(KEY_HOME)) { if (layers > 1) --layers; }
        else if (rlIsKeyPressed(KEY_END)) { if (layers < TEXT_MAX_LAYERS) ++layers; }

        // Handle text changes
        if (rlIsKeyPressed(KEY_LEFT)) fontSize -= 0.5f;
        else if (rlIsKeyPressed(KEY_RIGHT)) fontSize += 0.5f;
        else if (rlIsKeyPressed(KEY_UP)) fontSpacing -= 0.1f;
        else if (rlIsKeyPressed(KEY_DOWN)) fontSpacing += 0.1f;
        else if (rlIsKeyPressed(KEY_PAGE_UP)) lineSpacing -= 0.1f;
        else if (rlIsKeyPressed(KEY_PAGE_DOWN)) lineSpacing += 0.1f;
        else if (rlIsKeyDown(KEY_INSERT)) layerDistance -= 0.001f;
        else if (rlIsKeyDown(KEY_DELETE)) layerDistance += 0.001f;
        else if (rlIsKeyPressed(KEY_TAB))
        {
            multicolor = !multicolor;   // Enable /disable multicolor mode

            if (multicolor)
            {
                // Fill color array with random colors
                for (int i = 0; i < TEXT_MAX_LAYERS; ++i)
                {
                    multi[i] = GenerateRandomColor(0.5f, 0.8f);
                    multi[i].a = rlGetRandomValue(0, 255);
                }
            }
        }

        // Handle text input
        int ch = rlGetCharPressed();
        if (rlIsKeyPressed(KEY_BACKSPACE))
        {
            // Remove last char
            int len = rlTextLength(text);
            if (len > 0) text[len - 1] = '\0';
        }
        else if (rlIsKeyPressed(KEY_ENTER))
        {
            // handle newline
            int len = rlTextLength(text);
            if (len < sizeof(text) - 1)
            {
                text[len] = '\n';
                text[len+1] ='\0';
            }
        }
        else
        {
            // append only printable chars
            int len = rlTextLength(text);
            if (len < sizeof(text) - 1)
            {
                text[len] = ch;
                text[len+1] ='\0';
            }
        }

        // Measure 3D text so we can center it
        tbox = MeasureTextWave3D(font, text, fontSize, fontSpacing, lineSpacing);

        quads = 0;                      // Reset quad counter
        time += rlGetFrameTime();         // Update timer needed by `DrawTextWave3D()`
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(RAYWHITE);

            rlBeginMode3D(camera);
                rlDrawCubeV(cubePosition, cubeSize, dark);
                rlDrawCubeWires(cubePosition, 2.1f, 2.1f, 2.1f, light);

                rlDrawGrid(10, 2.0f);

                // Use a shader to handle the depth buffer issue with transparent textures
                // NOTE: more info at https://bedroomcoders.co.uk/raylib-billboards-advanced-use/
                rlBeginShaderMode(alphaDiscard);

                    // Draw the 3D text above the red cube
                    rlPushMatrix();
                        rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                        rlRotatef(90.0f, 0.0f, 0.0f, -1.0f);

                        for (int i = 0; i < layers; ++i)
                        {
                            rlColor clr = light;
                            if (multicolor) clr = multi[i];
                            DrawTextWave3D(font, text, (rlVector3){ -tbox.x/2.0f, layerDistance*i, -4.5f }, fontSize, fontSpacing, lineSpacing, true, &wcfg, time, clr);
                        }

                        // Draw the text boundry if set
                        if (SHOW_TEXT_BOUNDRY) rlDrawCubeWiresV((rlVector3){ 0.0f, 0.0f, -4.5f + tbox.z/2 }, tbox, dark);
                    rlPopMatrix();

                    // Don't draw the letter boundries for the 3D text below
                    bool slb = SHOW_LETTER_BOUNDRY;
                    SHOW_LETTER_BOUNDRY = false;

                    // Draw 3D options (use default font)
                    //-------------------------------------------------------------------------
                    rlPushMatrix();
                        rlRotatef(180.0f, 0.0f, 1.0f, 0.0f);
                        char *opt = (char *)rlTextFormat("< SIZE: %2.1f >", fontSize);
                        quads += rlTextLength(opt);
                        rlVector3 m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        rlVector3 pos = { -m.x/2.0f, 0.01f, 2.0f};
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, BLUE);
                        pos.z += 0.5f + m.z;

                        opt = (char *)rlTextFormat("< SPACING: %2.1f >", fontSpacing);
                        quads += rlTextLength(opt);
                        m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        pos.x = -m.x/2.0f;
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, BLUE);
                        pos.z += 0.5f + m.z;

                        opt = (char *)rlTextFormat("< LINE: %2.1f >", lineSpacing);
                        quads += rlTextLength(opt);
                        m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        pos.x = -m.x/2.0f;
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, BLUE);
                        pos.z += 1.0f + m.z;

                        opt = (char *)rlTextFormat("< LBOX: %3s >", slb? "ON" : "OFF");
                        quads += rlTextLength(opt);
                        m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        pos.x = -m.x/2.0f;
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, RED);
                        pos.z += 0.5f + m.z;

                        opt = (char *)rlTextFormat("< TBOX: %3s >", SHOW_TEXT_BOUNDRY? "ON" : "OFF");
                        quads += rlTextLength(opt);
                        m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        pos.x = -m.x/2.0f;
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, RED);
                        pos.z += 0.5f + m.z;

                        opt = (char *)rlTextFormat("< LAYER DISTANCE: %.3f >", layerDistance);
                        quads += rlTextLength(opt);
                        m = MeasureText3D(rlGetFontDefault(), opt, 8.0f, 1.0f, 0.0f);
                        pos.x = -m.x/2.0f;
                        DrawText3D(rlGetFontDefault(), opt, pos, 8.0f, 1.0f, 0.0f, false, DARKPURPLE);
                    rlPopMatrix();
                    //-------------------------------------------------------------------------

                    // Draw 3D info text (use default font)
                    //-------------------------------------------------------------------------
                    opt = "All the text displayed here is in 3D";
                    quads += 36;
                    m = MeasureText3D(rlGetFontDefault(), opt, 10.0f, 0.5f, 0.0f);
                    pos = (rlVector3){-m.x/2.0f, 0.01f, 2.0f};
                    DrawText3D(rlGetFontDefault(), opt, pos, 10.0f, 0.5f, 0.0f, false, DARKBLUE);
                    pos.z += 1.5f + m.z;

                    opt = "press [Left]/[Right] to change the font size";
                    quads += 44;
                    m = MeasureText3D(rlGetFontDefault(), opt, 6.0f, 0.5f, 0.0f);
                    pos.x = -m.x/2.0f;
                    DrawText3D(rlGetFontDefault(), opt, pos, 6.0f, 0.5f, 0.0f, false, DARKBLUE);
                    pos.z += 0.5f + m.z;

                    opt = "press [Up]/[Down] to change the font spacing";
                    quads += 44;
                    m = MeasureText3D(rlGetFontDefault(), opt, 6.0f, 0.5f, 0.0f);
                    pos.x = -m.x/2.0f;
                    DrawText3D(rlGetFontDefault(), opt, pos, 6.0f, 0.5f, 0.0f, false, DARKBLUE);
                    pos.z += 0.5f + m.z;

                    opt = "press [PgUp]/[PgDown] to change the line spacing";
                    quads += 48;
                    m = MeasureText3D(rlGetFontDefault(), opt, 6.0f, 0.5f, 0.0f);
                    pos.x = -m.x/2.0f;
                    DrawText3D(rlGetFontDefault(), opt, pos, 6.0f, 0.5f, 0.0f, false, DARKBLUE);
                    pos.z += 0.5f + m.z;

                    opt = "press [F1] to toggle the letter boundry";
                    quads += 39;
                    m = MeasureText3D(rlGetFontDefault(), opt, 6.0f, 0.5f, 0.0f);
                    pos.x = -m.x/2.0f;
                    DrawText3D(rlGetFontDefault(), opt, pos, 6.0f, 0.5f, 0.0f, false, DARKBLUE);
                    pos.z += 0.5f + m.z;

                    opt = "press [F2] to toggle the text boundry";
                    quads += 37;
                    m = MeasureText3D(rlGetFontDefault(), opt, 6.0f, 0.5f, 0.0f);
                    pos.x = -m.x/2.0f;
                    DrawText3D(rlGetFontDefault(), opt, pos, 6.0f, 0.5f, 0.0f, false, DARKBLUE);
                    //-------------------------------------------------------------------------

                    SHOW_LETTER_BOUNDRY = slb;
                rlEndShaderMode();

            rlEndMode3D();

            // Draw 2D info text & stats
            //-------------------------------------------------------------------------
            rlDrawText("Drag & drop a font file to change the font!\nType something, see what happens!\n\n"
            "Press [F3] to toggle the camera", 10, 35, 10, BLACK);

            quads += rlTextLength(text)*2*layers;
            char *tmp = (char *)rlTextFormat("%2i layer(s) | %s camera | %4i quads (%4i verts)", layers, spin? "ORBITAL" : "FREE", quads, quads*4);
            int width = rlMeasureText(tmp, 10);
            rlDrawText(tmp, screenWidth - 20 - width, 10, 10, DARKGREEN);

            tmp = "[Home]/[End] to add/remove 3D text layers";
            width = rlMeasureText(tmp, 10);
            rlDrawText(tmp, screenWidth - 20 - width, 25, 10, DARKGRAY);

            tmp = "[Insert]/[Delete] to increase/decrease distance between layers";
            width = rlMeasureText(tmp, 10);
            rlDrawText(tmp, screenWidth - 20 - width, 40, 10, DARKGRAY);

            tmp = "click the [CUBE] for a random color";
            width = rlMeasureText(tmp, 10);
            rlDrawText(tmp, screenWidth - 20 - width, 55, 10, DARKGRAY);

            tmp = "[Tab] to toggle multicolor mode";
            width = rlMeasureText(tmp, 10);
            rlDrawText(tmp, screenWidth - 20 - width, 70, 10, DARKGRAY);
            //-------------------------------------------------------------------------

            rlDrawFPS(10, 10);

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlUnloadFont(font);
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
// Module Functions Definitions
//--------------------------------------------------------------------------------------
// Draw codepoint at specified position in 3D space
static void DrawTextCodepoint3D(rlFont font, int codepoint, rlVector3 position, float fontSize, bool backface, rlColor tint)
{
    // Character index position in sprite font
    // NOTE: In case a codepoint is not available in the font, index returned points to '?'
    int index = rlGetGlyphIndex(font, codepoint);
    float scale = fontSize/(float)font.baseSize;

    // Character destination rectangle on screen
    // NOTE: We consider charsPadding on drawing
    position.x += (float)(font.glyphs[index].offsetX - font.glyphPadding)/(float)font.baseSize*scale;
    position.z += (float)(font.glyphs[index].offsetY - font.glyphPadding)/(float)font.baseSize*scale;

    // Character source rectangle from font texture atlas
    // NOTE: We consider chars padding when drawing, it could be required for outline/glow shader effects
    rlRectangle srcRec = { font.recs[index].x - (float)font.glyphPadding, font.recs[index].y - (float)font.glyphPadding,
                         font.recs[index].width + 2.0f*font.glyphPadding, font.recs[index].height + 2.0f*font.glyphPadding };

    float width = (float)(font.recs[index].width + 2.0f*font.glyphPadding)/(float)font.baseSize*scale;
    float height = (float)(font.recs[index].height + 2.0f*font.glyphPadding)/(float)font.baseSize*scale;

    if (font.texture.id > 0)
    {
        const float x = 0.0f;
        const float y = 0.0f;
        const float z = 0.0f;

        // normalized texture coordinates of the glyph inside the font texture (0.0f -> 1.0f)
        const float tx = srcRec.x/font.texture.width;
        const float ty = srcRec.y/font.texture.height;
        const float tw = (srcRec.x+srcRec.width)/font.texture.width;
        const float th = (srcRec.y+srcRec.height)/font.texture.height;

        if (SHOW_LETTER_BOUNDRY) rlDrawCubeWiresV((rlVector3){ position.x + width/2, position.y, position.z + height/2}, (rlVector3){ width, LETTER_BOUNDRY_SIZE, height }, LETTER_BOUNDRY_COLOR);

        rlCheckRenderBatchLimit(4 + 4*backface);
        rlSetTexture(font.texture.id);

        rlPushMatrix();
            rlTranslatef(position.x, position.y, position.z);

            rlBegin(RL_QUADS);
                rlColor4ub(tint.r, tint.g, tint.b, tint.a);

                // Front Face
                rlNormal3f(0.0f, 1.0f, 0.0f);                                   // Normal Pointing Up
                rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);              // Top Left Of The rlTexture and Quad
                rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height);     // Bottom Left Of The rlTexture and Quad
                rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);     // Bottom Right Of The rlTexture and Quad
                rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);              // Top Right Of The rlTexture and Quad

                if (backface)
                {
                    // Back Face
                    rlNormal3f(0.0f, -1.0f, 0.0f);                              // Normal Pointing Down
                    rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);          // Top Right Of The rlTexture and Quad
                    rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);          // Top Left Of The rlTexture and Quad
                    rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height); // Bottom Left Of The rlTexture and Quad
                    rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height); // Bottom Right Of The rlTexture and Quad
                }
            rlEnd();
        rlPopMatrix();

        rlSetTexture(0);
    }
}

// Draw a 2D text in 3D space
static void DrawText3D(rlFont font, const char *text, rlVector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, rlColor tint)
{
    int length = rlTextLength(text);          // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f;               // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;               // Offset X to next character to draw

    float scale = fontSize/(float)font.baseSize;

    for (int i = 0; i < length;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = rlGetCodepoint(&text[i], &codepointByteCount);
        int index = rlGetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += scale + lineSpacing/(float)font.baseSize*scale;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint3D(font, codepoint, (rlVector3){ position.x + textOffsetX, position.y, position.z + textOffsetY }, fontSize, backface, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += (float)(font.recs[index].width + fontSpacing)/(float)font.baseSize*scale;
            else textOffsetX += (float)(font.glyphs[index].advanceX + fontSpacing)/(float)font.baseSize*scale;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Measure a text in 3D. For some reason `rlMeasureTextEx()` just doesn't seem to work so i had to use this instead.
static rlVector3 MeasureText3D(rlFont font, const char* text, float fontSize, float fontSpacing, float lineSpacing)
{
    int len = rlTextLength(text);
    int tempLen = 0;                // Used to count longer text line num chars
    int lenCounter = 0;

    float tempTextWidth = 0.0f;     // Used to count longer text line width

    float scale = fontSize/(float)font.baseSize;
    float textHeight = scale;
    float textWidth = 0.0f;

    int letter = 0;                 // Current character
    int index = 0;                  // Index position in sprite font

    for (int i = 0; i < len; i++)
    {
        lenCounter++;

        int next = 0;
        letter = rlGetCodepoint(&text[i], &next);
        index = rlGetGlyphIndex(font, letter);

        // NOTE: normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol so to not skip any we set next = 1
        if (letter == 0x3f) next = 1;
        i += next - 1;

        if (letter != '\n')
        {
            if (font.glyphs[index].advanceX != 0) textWidth += (font.glyphs[index].advanceX+fontSpacing)/(float)font.baseSize*scale;
            else textWidth += (font.recs[index].width + font.glyphs[index].offsetX)/(float)font.baseSize*scale;
        }
        else
        {
            if (tempTextWidth < textWidth) tempTextWidth = textWidth;
            lenCounter = 0;
            textWidth = 0.0f;
            textHeight += scale + lineSpacing/(float)font.baseSize*scale;
        }

        if (tempLen < lenCounter) tempLen = lenCounter;
    }

    if (tempTextWidth < textWidth) tempTextWidth = textWidth;

    rlVector3 vec = { 0 };
    vec.x = tempTextWidth + (float)((tempLen - 1)*fontSpacing/(float)font.baseSize*scale); // Adds chars spacing to measure
    vec.y = 0.25f;
    vec.z = textHeight;

    return vec;
}

// Draw a 2D text in 3D space and wave the parts that start with `~~` and end with `~~`.
// This is a modified version of the original code by @Nighten found here https://github.com/NightenDushi/Raylib_DrawTextStyle
static void DrawTextWave3D(rlFont font, const char *text, rlVector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, WaveTextConfig* config, float time, rlColor tint)
{
    int length = rlTextLength(text);          // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f;               // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;               // Offset X to next character to draw

    float scale = fontSize/(float)font.baseSize;

    bool wave = false;

    for (int i = 0, k = 0; i < length; ++k)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = rlGetCodepoint(&text[i], &codepointByteCount);
        int index = rlGetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += scale + lineSpacing/(float)font.baseSize*scale;
            textOffsetX = 0.0f;
            k = 0;
        }
        else if (codepoint == '~')
        {
            if (rlGetCodepoint(&text[i+1], &codepointByteCount) == '~')
            {
                codepointByteCount += 1;
                wave = !wave;
            }
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                rlVector3 pos = position;
                if (wave) // Apply the wave effect
                {
                    pos.x += sinf(time*config->waveSpeed.x-k*config->waveOffset.x)*config->waveRange.x;
                    pos.y += sinf(time*config->waveSpeed.y-k*config->waveOffset.y)*config->waveRange.y;
                    pos.z += sinf(time*config->waveSpeed.z-k*config->waveOffset.z)*config->waveRange.z;
                }

                DrawTextCodepoint3D(font, codepoint, (rlVector3){ pos.x + textOffsetX, pos.y, pos.z + textOffsetY }, fontSize, backface, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += (float)(font.recs[index].width + fontSpacing)/(float)font.baseSize*scale;
            else textOffsetX += (float)(font.glyphs[index].advanceX + fontSpacing)/(float)font.baseSize*scale;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Measure a text in 3D ignoring the `~~` chars.
static rlVector3 MeasureTextWave3D(rlFont font, const char* text, float fontSize, float fontSpacing, float lineSpacing)
{
    int len = rlTextLength(text);
    int tempLen = 0;                // Used to count longer text line num chars
    int lenCounter = 0;

    float tempTextWidth = 0.0f;     // Used to count longer text line width

    float scale = fontSize/(float)font.baseSize;
    float textHeight = scale;
    float textWidth = 0.0f;

    int letter = 0;                 // Current character
    int index = 0;                  // Index position in sprite font

    for (int i = 0; i < len; i++)
    {
        lenCounter++;

        int next = 0;
        letter = rlGetCodepoint(&text[i], &next);
        index = rlGetGlyphIndex(font, letter);

        // NOTE: normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol so to not skip any we set next = 1
        if (letter == 0x3f) next = 1;
        i += next - 1;

        if (letter != '\n')
        {
            if (letter == '~' && rlGetCodepoint(&text[i+1], &next) == '~')
            {
                i++;
            }
            else
            {
                if (font.glyphs[index].advanceX != 0) textWidth += (font.glyphs[index].advanceX+fontSpacing)/(float)font.baseSize*scale;
                else textWidth += (font.recs[index].width + font.glyphs[index].offsetX)/(float)font.baseSize*scale;
            }
        }
        else
        {
            if (tempTextWidth < textWidth) tempTextWidth = textWidth;
            lenCounter = 0;
            textWidth = 0.0f;
            textHeight += scale + lineSpacing/(float)font.baseSize*scale;
        }

        if (tempLen < lenCounter) tempLen = lenCounter;
    }

    if (tempTextWidth < textWidth) tempTextWidth = textWidth;

    rlVector3 vec = { 0 };
    vec.x = tempTextWidth + (float)((tempLen - 1)*fontSpacing/(float)font.baseSize*scale); // Adds chars spacing to measure
    vec.y = 0.25f;
    vec.z = textHeight;

    return vec;
}

// Generates a nice color with a random hue
static rlColor GenerateRandomColor(float s, float v)
{
    const float Phi = 0.618033988749895f; // Golden ratio conjugate
    float h = (float)rlGetRandomValue(0, 360);
    h = fmodf((h + h*Phi), 360.0f);
    return rlColorFromHSV(h, s, v);
}
