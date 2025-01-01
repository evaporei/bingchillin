// this file is compiled and ran first when BUILD_RELEASE defined
#include <raylib.h>

int main()
{
    InitWindow(400, 400, "temp");
    Font font = LoadFont("monogram.ttf");
    ExportFontAsCode(font, "build/font.h");
    TraceLog(LOG_INFO, "font.h successfully generated");
    CloseWindow();
    return 0;
}
