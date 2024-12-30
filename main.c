#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define DA_IMPLEMENTATION
#include "dynamic_array.h"

#define LOG(...) TraceLog(LOG_DEBUG, TextFormat(__VA_ARGS__))

typedef struct {
    size_t start;
    size_t end;
} Line;

typedef struct {
    Line *items;
    size_t size;
    size_t count;
} Lines;

typedef struct {
    char *items;
    size_t size;
    size_t count;
} Buffer;

typedef struct {
    size_t cx;
    Buffer buffer;
    Lines  lines;
    size_t currentLine;

    int fontSize;
    Font font;
} Editor;

void editor_cursor_right(Editor *e)
{
    if (e->cx > e->buffer.count - 1) return;
    e->cx++;
}

void editor_cursor_left(Editor *e)
{
    if (e->cx == 0) return;
    e->cx--;
}

void editor_calculate_lines(Editor *e)
{
    da_free(&e->lines);
    Line line = {0};
    line.start = 0;
    for (size_t i=0; i<e->buffer.count; i++)
    {
        char c = e->buffer.items[i];
        if (c == '\n')
        {
            line.end = i;
            da_append(&e->lines, line);
            line.start = i+1;
        }
    }

    da_append(&e->lines, ((Line){
        line.start,
        e->buffer.count,
    }));
}

void editor_insert_char_at_cursor(Editor *e, char c)
{
    da_append(&e->buffer, '\0');
    // move memory from cursor to end right by one
    char *src = e->buffer.items + e->cx;
    char *dest = src + 1;
    size_t len = e->buffer.count - e->cx;

    memmove(dest, src, len);
    // set the char on cursor pos
    e->buffer.items[e->cx] = c;

    // move cursor right by one character
    e->cx++;

    editor_calculate_lines(e);
}

void editor_remove_char_before_cursor(Editor *e)
{
    if (e->cx == 0) return;

    char *src = e->buffer.items + e->cx;
    char *dest = e->buffer.items + e->cx - 1;
    size_t len = e->buffer.count - e->cx;
    memmove(dest, src, len);
    
    // set cursor position & buffer count
    if (e->cx > 0) e->cx--;
    if (e->buffer.count > 0) e->buffer.count--;

    editor_calculate_lines(e);
}

void editor_remove_char_at_cursor(Editor *e)
{
    if (e->buffer.count == 0) return;
    if (e->cx > e->buffer.count - 1) return;

    char *src = e->buffer.items + e->cx + 1;
    char *dest = e->buffer.items + e->cx;
    size_t len = e->buffer.count - (e->cx + 1);
    memmove(dest, src, len);

    if (e->buffer.count > 0)
        e->buffer.count--;

    editor_calculate_lines(e);
}

Editor ed = {0};

void init_editor()
{
    ed.cx = 0;
    ed.buffer = (Buffer) {0};
    da_init(&ed.buffer);
    ed.lines = (Lines) {0};
    da_init(&ed.lines);
    ed.currentLine = 0;

    ed.font = LoadFont("monogram.ttf");
    ed.fontSize = ed.font.baseSize;
}

void update()
{
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))
    {
        LOG("Cursor right");
        editor_cursor_right(&ed);
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))
    {
        LOG("Cursor left");
        editor_cursor_left(&ed);
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
    {
        if (IsKeyPressed(KEY_EQUAL)) ed.fontSize++;
        if (IsKeyPressed(KEY_MINUS)) ed.fontSize--;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        LOG("Enter key pressed");
        editor_insert_char_at_cursor(&ed, '\n');
    }

    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
    {
        LOG("Backspace pressed");
        editor_remove_char_before_cursor(&ed);
    }

    if (IsKeyPressed(KEY_DELETE) || IsKeyPressedRepeat(KEY_DELETE))
    {
        LOG("Delete pressed");
        editor_remove_char_at_cursor(&ed);
    }


    char key = GetCharPressed();
    if (key) {
        LOG("%c - character pressed", key);
        editor_insert_char_at_cursor(&ed, key);
    }
}

void draw()
{
        BeginDrawing();
        ClearBackground(BLACK);

        for (size_t i=0; i<ed.lines.count; i++)
        {
            Line line = ed.lines.items[i];

            const char* lineStart = ed.buffer.items + line.start;
            const size_t lineSize = line.end - line.start;
            char *text = malloc(lineSize + 1);
            memcpy(text, lineStart, lineSize);
            text[lineSize] = '\0';
            DrawTextEx(ed.font, text, (Vector2){0, ed.fontSize*i}, ed.fontSize, 2, GREEN);
            free(text);
        }
        EndDrawing();
}

int main()
{
    InitWindow(800, 600, "the bingchillin text editor");
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_DEBUG);

    init_editor();
    
    while(!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
    return 0;
}
