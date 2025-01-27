// this file is compiled and ran first when BUILD_RELEASE defined
#include <raylib.h>
#include <stdio.h>

int main()
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(400, 400, "temp");
    Font font = LoadFont("monogram.ttf");
    ExportFontAsCode(font, "build/font.h");
    printf("font.h successfully generated\n");
    CloseWindow();
    return 0;
}
