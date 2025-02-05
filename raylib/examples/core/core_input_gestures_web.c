/*******************************************************************************************
*
*   raylib [core] example - Input Gestures for Web
*
*   Example originally created with raylib 4.6-dev, last time updated with raylib 4.6-dev
*
*   Example contributed by ubkp (@ubkp) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 ubkp (@ubkp)
*
********************************************************************************************/

#include "raylib.h"

#include "math.h"       // Required for the protractor angle graphic drawing

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h> // Required for the Web/HTML5
#endif

//--------------------------------------------------------------------------------------
// Global definitions and declarations
//--------------------------------------------------------------------------------------

// Common variables definitions
//--------------------------------------------------------------------------------------
int screenWidth = 800;                  // Update depending on web canvas
const int screenHeight = 450;
rlVector2 messagePosition = { 160, 7 };

// Last gesture variables definitions
//--------------------------------------------------------------------------------------
int lastGesture = 0;
rlVector2 lastGesturePosition = { 165, 130 };

// Gesture log variables definitions and functions declarations
//--------------------------------------------------------------------------------------
#define GESTURE_LOG_SIZE 20
char gestureLog[GESTURE_LOG_SIZE][12] = { "" }; // The gesture log uses an array (as an inverted circular queue) to store the performed gestures
int gestureLogIndex = GESTURE_LOG_SIZE;         // The index for the inverted circular queue (moving from last to first direction, then looping around)
int previousGesture = 0;

char const *GetGestureName(int i)
{
   switch (i)  {
      case 0:   return "None";        break;
      case 1:   return "Tap";         break;
      case 2:   return "Double Tap";  break;
      case 4:   return "Hold";        break;
      case 8:   return "Drag";        break;
      case 16:  return "Swipe Right"; break;
      case 32:  return "Swipe Left";  break;
      case 64:  return "Swipe Up";    break;
      case 128: return "Swipe Down";  break;
      case 256: return "Pinch In";    break;
      case 512: return "Pinch Out";   break;
      default:  return "Unknown";     break;
   }
}

rlColor GetGestureColor(int i)
{
   switch (i)  {
      case 0:   return BLACK;   break;
      case 1:   return BLUE;    break;
      case 2:   return SKYBLUE; break;
      case 4:   return BLACK;   break;
      case 8:   return LIME;    break;
      case 16:  return RED;     break;
      case 32:  return RED;     break;
      case 64:  return RED;     break;
      case 128: return RED;     break;
      case 256: return VIOLET;  break;
      case 512: return ORANGE;  break;
      default:  return BLACK;   break;
   }
}

int logMode = 1; // Log mode values: 0 shows repeated events; 1 hides repeated events; 2 shows repeated events but hide hold events; 3 hides repeated events and hide hold events

rlColor gestureColor = { 0, 0, 0, 255 };
rlRectangle logButton1 = { 53, 7, 48, 26 };
rlRectangle logButton2 = { 108, 7, 36, 26 };
rlVector2 gestureLogPosition = { 10, 10 };

// Protractor variables definitions
//--------------------------------------------------------------------------------------
float angleLength = 90.0f;
float currentAngleDegrees = 0.0f;
rlVector2 finalVector = { 0.0f, 0.0f };
char currentAngleStr[7] = "";
rlVector2 protractorPosition = { 266.0f, 315.0f };

// Update
//--------------------------------------------------------------------------------------
void Update(void)
{
    // Handle common
    //--------------------------------------------------------------------------------------
    int i, ii; // Iterators that will be reused by all for loops
    const int currentGesture = rlGetGestureDetected();
    const float currentDragDegrees = rlGetGestureDragAngle();
    const float currentPitchDegrees = rlGetGesturePinchAngle();
    const int touchCount = rlGetTouchPointCount();

    // Handle last gesture
    //--------------------------------------------------------------------------------------
    if ((currentGesture != 0) && (currentGesture != 4) && (currentGesture != previousGesture)) lastGesture = currentGesture; // Filter the meaningful gestures (1, 2, 8 to 512) for the display

    // Handle gesture log
    //--------------------------------------------------------------------------------------
    if (rlIsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        if (rlCheckCollisionPointRec(rlGetMousePosition(), logButton1))
        {
            switch (logMode)
            {
                case 3:  logMode=2; break;
                case 2:  logMode=3; break;
                case 1:  logMode=0; break;
                default: logMode=1; break;
            }
        }
        else if (rlCheckCollisionPointRec(rlGetMousePosition(), logButton2))
        {
            switch (logMode)
            {
                case 3:  logMode=1; break;
                case 2:  logMode=0; break;
                case 1:  logMode=3; break;
                default: logMode=2; break;
            }
        }
    }

    int fillLog = 0; // Gate variable to be used to allow or not the gesture log to be filled
    if (currentGesture !=0)
    {
        if (logMode == 3) // 3 hides repeated events and hide hold events
        {
            if (((currentGesture != 4) && (currentGesture != previousGesture)) || (currentGesture < 3)) fillLog = 1;
        }
        else if (logMode == 2) // 2 shows repeated events but hide hold events
        {
            if (currentGesture != 4) fillLog = 1;
        }
        else if (logMode == 1) // 1 hides repeated events
        {
            if (currentGesture != previousGesture) fillLog = 1;
        }
        else  // 0 shows repeated events
        {
            fillLog = 1;
        }
    }

    if (fillLog) // If one of the conditions from logMode was met, fill the gesture log
    {
        previousGesture = currentGesture;
        gestureColor = GetGestureColor(currentGesture);
        if (gestureLogIndex <= 0) gestureLogIndex = GESTURE_LOG_SIZE;
        gestureLogIndex--;

        // Copy the gesture respective name to the gesture log array
        rlTextCopy(gestureLog[gestureLogIndex], GetGestureName(currentGesture));
    }

    // Handle protractor
    //--------------------------------------------------------------------------------------
    if (currentGesture > 255) // aka Pinch In and Pinch Out
    {
        currentAngleDegrees = currentPitchDegrees;
    }
    else if (currentGesture > 15) // aka Swipe Right, Swipe Left, Swipe Up and Swipe Down
    {
        currentAngleDegrees = currentDragDegrees;
    }
    else if (currentGesture > 0) // aka Tap, Doubletap, Hold and Grab
    {
        currentAngleDegrees = 0.0f;
    }

    float currentAngleRadians = ((currentAngleDegrees +90.0f)*PI/180); // Convert the current angle to Radians
    finalVector = (rlVector2){ (angleLength*sinf(currentAngleRadians)) + protractorPosition.x, (angleLength*cosf(currentAngleRadians)) + protractorPosition.y }; // Calculate the final vector for display

    // Handle touch and mouse pointer points
    //--------------------------------------------------------------------------------------
    #define MAX_TOUCH_COUNT     32

    rlVector2 touchPosition[MAX_TOUCH_COUNT] = { 0 };
    rlVector2 mousePosition = {0, 0};
    if (currentGesture != GESTURE_NONE)
    {
        if (touchCount != 0)
        {
            for (i = 0; i < touchCount; i++) touchPosition[i] = rlGetTouchPosition(i); // Fill the touch positions
        }
        else mousePosition = rlGetMousePosition();
    }

    // Draw
    //--------------------------------------------------------------------------------------
    rlBeginDrawing();

        rlClearBackground(RAYWHITE);

        // Draw common
        //--------------------------------------------------------------------------------------
        rlDrawText("*", messagePosition.x + 5, messagePosition.y + 5, 10, BLACK);
        rlDrawText("Example optimized for Web/HTML5\non Smartphones with Touch Screen.", messagePosition.x + 15, messagePosition.y + 5, 10, BLACK);
        rlDrawText("*", messagePosition.x + 5, messagePosition.y + 35, 10, BLACK);
        rlDrawText("While running on Desktop Web Browsers,\ninspect and turn on Touch Emulation.", messagePosition.x + 15,  messagePosition.y + 35, 10, BLACK);

        // Draw last gesture
        //--------------------------------------------------------------------------------------
        rlDrawText("Last gesture", lastGesturePosition.x + 33, lastGesturePosition.y - 47, 20, BLACK);
        rlDrawText("Swipe         Tap       Pinch  Touch", lastGesturePosition.x + 17, lastGesturePosition.y - 18, 10, BLACK);
        rlDrawRectangle(lastGesturePosition.x + 20, lastGesturePosition.y, 20, 20, lastGesture == GESTURE_SWIPE_UP ? RED : LIGHTGRAY);
        rlDrawRectangle(lastGesturePosition.x, lastGesturePosition.y + 20, 20, 20, lastGesture == GESTURE_SWIPE_LEFT ? RED : LIGHTGRAY);
        rlDrawRectangle(lastGesturePosition.x + 40, lastGesturePosition.y + 20, 20, 20, lastGesture == GESTURE_SWIPE_RIGHT ? RED : LIGHTGRAY);
        rlDrawRectangle(lastGesturePosition.x + 20, lastGesturePosition.y + 40, 20, 20, lastGesture == GESTURE_SWIPE_DOWN ? RED : LIGHTGRAY);
        rlDrawCircle(lastGesturePosition.x + 80, lastGesturePosition.y + 16, 10, lastGesture == GESTURE_TAP ? BLUE : LIGHTGRAY);
        rlDrawRing( (rlVector2){lastGesturePosition.x + 103, lastGesturePosition.y + 16}, 6.0f, 11.0f, 0.0f, 360.0f, 0, lastGesture == GESTURE_DRAG ? LIME : LIGHTGRAY);
        rlDrawCircle(lastGesturePosition.x + 80, lastGesturePosition.y + 43, 10, lastGesture == GESTURE_DOUBLETAP ? SKYBLUE : LIGHTGRAY);
        rlDrawCircle(lastGesturePosition.x + 103, lastGesturePosition.y + 43, 10, lastGesture == GESTURE_DOUBLETAP ? SKYBLUE : LIGHTGRAY);
        rlDrawTriangle((rlVector2){ lastGesturePosition.x + 122, lastGesturePosition.y + 16 }, (rlVector2){ lastGesturePosition.x + 137, lastGesturePosition.y + 26 }, (rlVector2){ lastGesturePosition.x + 137, lastGesturePosition.y + 6 }, lastGesture == GESTURE_PINCH_OUT? ORANGE : LIGHTGRAY);
        rlDrawTriangle((rlVector2){ lastGesturePosition.x + 147, lastGesturePosition.y + 6 }, (rlVector2){ lastGesturePosition.x + 147, lastGesturePosition.y + 26 }, (rlVector2){ lastGesturePosition.x + 162, lastGesturePosition.y + 16 }, lastGesture == GESTURE_PINCH_OUT? ORANGE : LIGHTGRAY);
        rlDrawTriangle((rlVector2){ lastGesturePosition.x + 125, lastGesturePosition.y + 33 }, (rlVector2){ lastGesturePosition.x + 125, lastGesturePosition.y + 53 }, (rlVector2){ lastGesturePosition.x + 140, lastGesturePosition.y + 43 }, lastGesture == GESTURE_PINCH_IN? VIOLET : LIGHTGRAY);
        rlDrawTriangle((rlVector2){ lastGesturePosition.x + 144, lastGesturePosition.y + 43 }, (rlVector2){ lastGesturePosition.x + 159, lastGesturePosition.y + 53 }, (rlVector2){ lastGesturePosition.x + 159, lastGesturePosition.y + 33 }, lastGesture == GESTURE_PINCH_IN? VIOLET : LIGHTGRAY);
        for (i = 0; i < 4; i++) rlDrawCircle(lastGesturePosition.x + 180, lastGesturePosition.y + 7 + i*15, 5, touchCount <= i? LIGHTGRAY : gestureColor);

        // Draw gesture log
        //--------------------------------------------------------------------------------------
        rlDrawText("Log", gestureLogPosition.x, gestureLogPosition.y, 20, BLACK);

        // Loop in both directions to print the gesture log array in the inverted order (and looping around if the index started somewhere in the middle)
        for (i = 0, ii = gestureLogIndex; i < GESTURE_LOG_SIZE; i++, ii = (ii + 1) % GESTURE_LOG_SIZE) rlDrawText(gestureLog[ii], gestureLogPosition.x, gestureLogPosition.y + 410 - i*20, 20, (i == 0 ? gestureColor : LIGHTGRAY));
        rlColor logButton1Color, logButton2Color;
        switch (logMode)
        {
            case 3:  logButton1Color=MAROON; logButton2Color=MAROON; break;
            case 2:  logButton1Color=GRAY;   logButton2Color=MAROON; break;
            case 1:  logButton1Color=MAROON; logButton2Color=GRAY;   break;
            default: logButton1Color=GRAY;   logButton2Color=GRAY;   break;
        }
        rlDrawRectangleRec(logButton1, logButton1Color);
        rlDrawText("Hide", logButton1.x + 7, logButton1.y + 3, 10, WHITE);
        rlDrawText("Repeat", logButton1.x + 7, logButton1.y + 13, 10, WHITE);
        rlDrawRectangleRec(logButton2, logButton2Color);
        rlDrawText("Hide", logButton1.x + 62, logButton1.y + 3, 10, WHITE);
        rlDrawText("Hold", logButton1.x + 62, logButton1.y + 13, 10, WHITE);

        // Draw protractor
        //--------------------------------------------------------------------------------------
        rlDrawText("Angle", protractorPosition.x + 55, protractorPosition.y + 76, 10, BLACK);
        const char *angleString = rlTextFormat("%f", currentAngleDegrees);
        const int angleStringDot = rlTextFindIndex(angleString, ".");
        const char *angleStringTrim = rlTextSubtext(angleString, 0, angleStringDot + 3);
        rlDrawText( angleStringTrim, protractorPosition.x + 55, protractorPosition.y + 92, 20, gestureColor);
        rlDrawCircle(protractorPosition.x, protractorPosition.y, 80.0f, WHITE);
        rlDrawLineEx((rlVector2){ protractorPosition.x - 90, protractorPosition.y }, (rlVector2){ protractorPosition.x + 90, protractorPosition.y }, 3.0f, LIGHTGRAY);
        rlDrawLineEx((rlVector2){ protractorPosition.x, protractorPosition.y - 90 }, (rlVector2){ protractorPosition.x, protractorPosition.y + 90 }, 3.0f, LIGHTGRAY);
        rlDrawLineEx((rlVector2){ protractorPosition.x - 80, protractorPosition.y - 45 }, (rlVector2){ protractorPosition.x + 80, protractorPosition.y + 45 }, 3.0f, GREEN);
        rlDrawLineEx((rlVector2){ protractorPosition.x - 80, protractorPosition.y + 45 }, (rlVector2){ protractorPosition.x + 80, protractorPosition.y - 45 }, 3.0f, GREEN);
        rlDrawText("0", protractorPosition.x + 96, protractorPosition.y - 9, 20, BLACK);
        rlDrawText("30", protractorPosition.x + 74, protractorPosition.y - 68, 20, BLACK);
        rlDrawText("90", protractorPosition.x - 11, protractorPosition.y - 110, 20, BLACK);
        rlDrawText("150", protractorPosition.x - 100, protractorPosition.y - 68, 20, BLACK);
        rlDrawText("180", protractorPosition.x - 124, protractorPosition.y - 9, 20, BLACK);
        rlDrawText("210", protractorPosition.x - 100, protractorPosition.y + 50, 20, BLACK);
        rlDrawText("270", protractorPosition.x - 18, protractorPosition.y + 92, 20, BLACK);
        rlDrawText("330", protractorPosition.x + 72, protractorPosition.y + 50, 20, BLACK);
        if (currentAngleDegrees != 0.0f) rlDrawLineEx(protractorPosition, finalVector, 3.0f, gestureColor);

        // Draw touch and mouse pointer points
        //--------------------------------------------------------------------------------------
        if (currentGesture != GESTURE_NONE)
        {
            if ( touchCount != 0 )
            {
                for (i = 0; i < touchCount; i++)
                {
                    rlDrawCircleV(touchPosition[i], 50.0f, rlFade(gestureColor, 0.5f));
                    rlDrawCircleV(touchPosition[i], 5.0f, gestureColor);
                }

                if (touchCount == 2) rlDrawLineEx(touchPosition[0], touchPosition[1], ((currentGesture == 512)? 8 : 12), gestureColor);
            }
            else
            {
                rlDrawCircleV(mousePosition, 35.0f, rlFade(gestureColor, 0.5f));
                rlDrawCircleV(mousePosition, 5.0f, gestureColor);
            }
        }

    rlEndDrawing();
    //--------------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    rlInitWindow(screenWidth, screenHeight, "raylib [core] example - input gestures web");
    //--------------------------------------------------------------------------------------

    // Main game loop
    //--------------------------------------------------------------------------------------
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(Update, 0, 1);
    #else
        rlSetTargetFPS(60);
        while (!rlWindowShouldClose()) Update(); // Detect window close button or ESC key
    #endif
    //--------------------------------------------------------------------------------------

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlCloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
