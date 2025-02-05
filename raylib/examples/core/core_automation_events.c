/*******************************************************************************************
*
*   raylib [core] example - automation events
*
*   Example originally created with raylib 5.0, last time updated with raylib 5.0
*
*   Example based on 2d_camera_platformer example by arvyy (@arvyy)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#define GRAVITY 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

#define MAX_ENVIRONMENT_ELEMENTS    5

typedef struct Player {
    rlVector2 position;
    float speed;
    bool canJump;
} Player;

typedef struct EnvElement {
    rlRectangle rect;
    int blocking;
    rlColor color;
} EnvElement;


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - automation events");

    // Define player
    Player player = { 0 };
    player.position = (rlVector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    
    // Define environment elements (platforms)
    EnvElement envElements[MAX_ENVIRONMENT_ELEMENTS] = {
        {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
        {{ 0, 400, 1000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
    };

    // Define camera
    rlCamera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    // Automation events
    rlAutomationEventList aelist = rlLoadAutomationEventList(0);  // Initialize list of automation events to record new events
    rlSetAutomationEventList(&aelist);
    bool eventRecording = false;
    bool eventPlaying = false;
    
    unsigned int frameCounter = 0;
    unsigned int playFrameCounter = 0;
    unsigned int currentPlayFrame = 0;

    rlSetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!rlWindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = 0.015f;//rlGetFrameTime();
       
        // Dropped files logic
        //----------------------------------------------------------------------------------
        if (rlIsFileDropped())
        {
            rlFilePathList droppedFiles = rlLoadDroppedFiles();

            // Supports loading .rgs style files (text or binary) and .png style palette images
            if (rlIsFileExtension(droppedFiles.paths[0], ".txt;.rae"))
            {
                rlUnloadAutomationEventList(aelist);
                aelist = rlLoadAutomationEventList(droppedFiles.paths[0]);
                
                eventRecording = false;
                
                // Reset scene state to play
                eventPlaying = true;
                playFrameCounter = 0;
                currentPlayFrame = 0;
                
                player.position = (rlVector2){ 400, 280 };
                player.speed = 0;
                player.canJump = false;

                camera.target = player.position;
                camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
                camera.rotation = 0.0f;
                camera.zoom = 1.0f;
            }

            rlUnloadDroppedFiles(droppedFiles);   // Unload filepaths from memory
        }
        //----------------------------------------------------------------------------------

        // Update player
        //----------------------------------------------------------------------------------
        if (rlIsKeyDown(KEY_LEFT)) player.position.x -= PLAYER_HOR_SPD*deltaTime;
        if (rlIsKeyDown(KEY_RIGHT)) player.position.x += PLAYER_HOR_SPD*deltaTime;
        if (rlIsKeyDown(KEY_SPACE) && player.canJump)
        {
            player.speed = -PLAYER_JUMP_SPD;
            player.canJump = false;
        }

        int hitObstacle = 0;
        for (int i = 0; i < MAX_ENVIRONMENT_ELEMENTS; i++)
        {
            EnvElement *element = &envElements[i];
            rlVector2 *p = &(player.position);
            if (element->blocking &&
                element->rect.x <= p->x &&
                element->rect.x + element->rect.width >= p->x &&
                element->rect.y >= p->y &&
                element->rect.y <= p->y + player.speed*deltaTime)
            {
                hitObstacle = 1;
                player.speed = 0.0f;
                p->y = element->rect.y;
            }
        }

        if (!hitObstacle)
        {
            player.position.y += player.speed*deltaTime;
            player.speed += GRAVITY*deltaTime;
            player.canJump = false;
        }
        else player.canJump = true;

        if (rlIsKeyPressed(KEY_R))
        {
            // Reset game state
            player.position = (rlVector2){ 400, 280 };
            player.speed = 0;
            player.canJump = false;

            camera.target = player.position;
            camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
            camera.rotation = 0.0f;
            camera.zoom = 1.0f;
        }
        //----------------------------------------------------------------------------------

        // Events playing
        // NOTE: Logic must be before Camera update because it depends on mouse-wheel value, 
        // that can be set by the played event... but some other inputs could be affected
        //----------------------------------------------------------------------------------
        if (eventPlaying)
        {
            // NOTE: Multiple events could be executed in a single frame
            while (playFrameCounter == aelist.events[currentPlayFrame].frame)
            {
                rlPlayAutomationEvent(aelist.events[currentPlayFrame]);
                currentPlayFrame++;

                if (currentPlayFrame == aelist.count)
                {
                    eventPlaying = false;
                    currentPlayFrame = 0;
                    playFrameCounter = 0;

                    rlTraceLog(LOG_INFO, "FINISH PLAYING!");
                    break;
                }
            }

            playFrameCounter++;
        }
        //----------------------------------------------------------------------------------

        // Update camera
        //----------------------------------------------------------------------------------
        camera.target = player.position;
        camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
        float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

        // WARNING: On event replay, mouse-wheel internal value is set
        camera.zoom += ((float)rlGetMouseWheelMove()*0.05f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        for (int i = 0; i < MAX_ENVIRONMENT_ELEMENTS; i++)
        {
            EnvElement *element = &envElements[i];
            minX = fminf(element->rect.x, minX);
            maxX = fmaxf(element->rect.x + element->rect.width, maxX);
            minY = fminf(element->rect.y, minY);
            maxY = fmaxf(element->rect.y + element->rect.height, maxY);
        }

        rlVector2 max = rlGetWorldToScreen2D((rlVector2){ maxX, maxY }, camera);
        rlVector2 min = rlGetWorldToScreen2D((rlVector2){ minX, minY }, camera);

        if (max.x < screenWidth) camera.offset.x = screenWidth - (max.x - screenWidth/2);
        if (max.y < screenHeight) camera.offset.y = screenHeight - (max.y - screenHeight/2);
        if (min.x > 0) camera.offset.x = screenWidth/2 - min.x;
        if (min.y > 0) camera.offset.y = screenHeight/2 - min.y;
        //----------------------------------------------------------------------------------
        
        // Events management
        if (rlIsKeyPressed(KEY_S))    // Toggle events recording
        {
            if (!eventPlaying)
            {
                if (eventRecording)
                {
                    rlStopAutomationEventRecording();
                    eventRecording = false;
                    
                    rlExportAutomationEventList(aelist, "automation.rae");
                    
                    rlTraceLog(LOG_INFO, "RECORDED FRAMES: %i", aelist.count);
                }
                else 
                {
                    rlSetAutomationEventBaseFrame(180);
                    rlStartAutomationEventRecording();
                    eventRecording = true;
                }
            }
        }
        else if (rlIsKeyPressed(KEY_A)) // Toggle events playing (WARNING: Starts next frame)
        {
            if (!eventRecording && (aelist.count > 0))
            {
                // Reset scene state to play
                eventPlaying = true;
                playFrameCounter = 0;
                currentPlayFrame = 0;

                player.position = (rlVector2){ 400, 280 };
                player.speed = 0;
                player.canJump = false;

                camera.target = player.position;
                camera.offset = (rlVector2){ screenWidth/2.0f, screenHeight/2.0f };
                camera.rotation = 0.0f;
                camera.zoom = 1.0f;
            }
        }

        if (eventRecording || eventPlaying) frameCounter++;
        else frameCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlBeginDrawing();

            rlClearBackground(LIGHTGRAY);

            rlBeginMode2D(camera);

                // Draw environment elements
                for (int i = 0; i < MAX_ENVIRONMENT_ELEMENTS; i++)
                {
                    rlDrawRectangleRec(envElements[i].rect, envElements[i].color);
                }

                // Draw player rectangle
                rlDrawRectangleRec((rlRectangle){ player.position.x - 20, player.position.y - 40, 40, 40 }, RED);

            rlEndMode2D();
            
            // Draw game controls
            rlDrawRectangle(10, 10, 290, 145, rlFade(SKYBLUE, 0.5f));
            rlDrawRectangleLines(10, 10, 290, 145, rlFade(BLUE, 0.8f));

            rlDrawText("Controls:", 20, 20, 10, BLACK);
            rlDrawText("- RIGHT | LEFT: Player movement", 30, 40, 10, DARKGRAY);
            rlDrawText("- SPACE: Player jump", 30, 60, 10, DARKGRAY);
            rlDrawText("- R: Reset game state", 30, 80, 10, DARKGRAY);

            rlDrawText("- S: START/STOP RECORDING INPUT EVENTS", 30, 110, 10, BLACK);
            rlDrawText("- A: REPLAY LAST RECORDED INPUT EVENTS", 30, 130, 10, BLACK);

            // Draw automation events recording indicator
            if (eventRecording)
            {
                rlDrawRectangle(10, 160, 290, 30, rlFade(RED, 0.3f));
                rlDrawRectangleLines(10, 160, 290, 30, rlFade(MAROON, 0.8f));
                rlDrawCircle(30, 175, 10, MAROON);

                if (((frameCounter/15)%2) == 1) rlDrawText(rlTextFormat("RECORDING EVENTS... [%i]", aelist.count), 50, 170, 10, MAROON);
            }
            else if (eventPlaying)
            {
                rlDrawRectangle(10, 160, 290, 30, rlFade(LIME, 0.3f));
                rlDrawRectangleLines(10, 160, 290, 30, rlFade(DARKGREEN, 0.8f));
                rlDrawTriangle((rlVector2){ 20, 155 + 10 }, (rlVector2){ 20, 155 + 30 }, (rlVector2){ 40, 155 + 20 }, DARKGREEN);

                if (((frameCounter/15)%2) == 1) rlDrawText(rlTextFormat("PLAYING RECORDED EVENTS... [%i]", currentPlayFrame), 50, 170, 10, DARKGREEN);
            }
            

        rlEndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
