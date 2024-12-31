#include <assert.h>
#include <stdbool.h>
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
    size_t row;

    int scrollX;
    int scrollY;
    
    const char * filename;

    int fontSize;
    int charWidth;
    Font font;
} Editor;

size_t editor_get_cursor_row(Editor *e)
{
    assert(e->lines.count > 0);

    for (size_t i=0; i<e->lines.count; i++)
    {
        Line line = e->lines.items[i];
        if (e->cx >= line.start && e->cx <= line.end)
            return i;
    }
    // return the last line/row
    return e->lines.count - 1;
}

void editor_cursor_right(Editor *e)
{
    if (e->buffer.count == 0) return;
    if (e->cx > e->buffer.count - 1) return;
    e->cx++;
}

void editor_cursor_left(Editor *e)
{
    if (e->cx == 0) return;
    e->cx--;
}

void editor_cursor_down(Editor *e)
{
    Line line = e->lines.items[e->row];
    size_t col = e->cx - line.start;

    if (e->row+1 > e->lines.count - 1) return;

    Line nextLine = e->lines.items[e->row+1];
    size_t nextLineSize = nextLine.end - nextLine.start;

    if (nextLineSize >= col)
        e->cx = nextLine.start + col;
    else
        e->cx = nextLine.end;
}

void editor_cursor_up(Editor *e)
{
    Line line = e->lines.items[e->row];
    size_t col = e->cx - line.start;

    if (e->row == 0) return;

    Line prevLine = e->lines.items[e->row-1];
    size_t prevLineSize = prevLine.end - prevLine.start;

    if (prevLineSize >= col)
        e->cx = prevLine.start + col;
    else
        e->cx = prevLine.end;
}

void editor_cursor_to_next_word(Editor *e)
{
    bool foundWhitespace = false;

    for (size_t i=e->cx; i<e->buffer.count; i++)
    {
        const char c = e->buffer.items[i];
        const bool checkWhitespace = c==' ' || c=='\n';;

        if (checkWhitespace)
            foundWhitespace = true;

        // check if char is whitespace
        if (foundWhitespace && !checkWhitespace)
        {
            e->cx = i;
            return;
        }
    }
    // if no next word found
    const Line line = e->lines.items[e->row];
    e->cx = line.end;
}

void editor_cursor_to_prev_word(Editor *e)
{
    bool foundWhitespace = false;
    
    for (size_t i=e->cx; i!=0; i--)
    {
        const char c = e->buffer.items[i-1];
        const bool checkWhitespace = c==' ' || c=='\n';;

        if (checkWhitespace)
            foundWhitespace = true;

        // check if char is whitespace
        if (foundWhitespace && !checkWhitespace)
        {
            e->cx = i;
            return;
        }
    }
    // no prev word found
    const Line line = e->lines.items[e->row];
    e->cx = line.start;
}

void editor_cursor_to_line_start(Editor *e)
{
    Line line = e->lines.items[e->row];
    e->cx = line.start;
}

void editor_cursor_to_line_end(Editor *e)
{
    Line line = e->lines.items[e->row];
    e->cx = line.end;
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

    // there's always atleast one line 
    // a lot of code depends upon that assumption
    da_append(&e->lines, ((Line){
        line.start,
        e->buffer.count,
    }));
}

// Initialize Editor struct
void editor_init(Editor *e)
{
    e->cx = 0;
    e->buffer = (Buffer) {0};
    da_init(&e->buffer);
    e->lines = (Lines) {0};
    da_init(&e->lines);
    e->row = 0;

    e->scrollX = 0;
    e->scrollY = 0;
    
    e->filename = NULL;

    e->font = LoadFont("monogram.ttf");
    e->fontSize = e->font.baseSize;
    e->charWidth = MeasureTextEx(e->font, "a", e->fontSize, 0).x;

    editor_calculate_lines(e);
}

void editor_deinit(Editor *e)
{
    da_free(&e->buffer);
    da_free(&e->lines);
    UnloadFont(e->font);
}

void editor_insert_char_at_cursor(Editor *e, char c)
{
    da_append(&e->buffer, '\0'); // increases count by one
    // move memory from cursor to end right by one
    char *src = e->buffer.items + e->cx;
    char *dest = src + 1;
    size_t len = e->buffer.count - 1 - e->cx;

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

void editor_load_file(Editor *e, const char *filename)
{
    LOG("Opening file: %s", filename);

    // get size of the file
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    long int size = ftell(f);
    rewind(f); // set cursor back to beginning?
    LOG("size of file(%s):%ld\n", filename, size);
    
    // allocate that much memory in the buffer
    da_reserve(&e->buffer, (size_t)size);

    // copy file's contents to buffer
    fread(e->buffer.items, 1, size, f);
    e->buffer.count = size;

    // remember to close file
    fclose(f);

    e->filename = filename;
    editor_calculate_lines(e);
}

void editor_save_file(Editor *e)
{
    LOG("Saving to filename: %s", e->filename);
    if (e->filename == NULL) return;

    // open file for writing
    FILE *f = fopen(e->filename, "r+");
    if (f == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    // write the buffer to the previously opened file
    fwrite(e->buffer.items, 1, e->buffer.count, f);

    fclose(f);
}

bool editor_key_pressed(KeyboardKey key)
{
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

void update(Editor *e)
{
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        if (editor_key_pressed(KEY_EQUAL)) e->fontSize++;
        if (editor_key_pressed(KEY_MINUS)) e->fontSize--;
        if (IsKeyPressed(KEY_S)) editor_save_file(e);
    }

    if (editor_key_pressed(KEY_RIGHT))
    {
        LOG("Cursor right");
        if (IsKeyDown(KEY_LEFT_CONTROL)) editor_cursor_to_next_word(e);
        else editor_cursor_right(e);
    }

    if (editor_key_pressed(KEY_LEFT))
    {
        LOG("Cursor left");
        if (IsKeyDown(KEY_LEFT_CONTROL)) editor_cursor_to_prev_word(e);
        else editor_cursor_left(e);
    }

    if (editor_key_pressed(KEY_DOWN))
    {
        LOG("Cursor down");
        editor_cursor_down(e);
    }

    if (editor_key_pressed(KEY_UP))
    {
        LOG("Cursor up");
        editor_cursor_up(e);
    }

    if (IsKeyPressed(KEY_HOME))
    {
        LOG("Home key pressed");
        editor_cursor_to_line_start(e);
    }

    if (IsKeyPressed(KEY_END))
    {
        LOG("End key pressed");
        editor_cursor_to_line_end(e);
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        LOG("Enter key pressed");
        editor_insert_char_at_cursor(e, '\n');
    }

    if (IsKeyPressed(KEY_TAB))
    {
        LOG("Tab key pressed");
        for (int i=0; i<4; i++) editor_insert_char_at_cursor(e, ' ');
    }

    if (editor_key_pressed(KEY_BACKSPACE))
    {
        LOG("Backspace pressed");
        editor_remove_char_before_cursor(e);
    }

    if (editor_key_pressed(KEY_DELETE))
    {
        LOG("Delete pressed");
        editor_remove_char_at_cursor(e);
    }


    char key = GetCharPressed();
    if (key) {
        LOG("%c - character pressed", key);
        editor_insert_char_at_cursor(e, key);
    }
    
    
    { // Update Editor members
        e->row = editor_get_cursor_row(e);
        e->charWidth = MeasureTextEx(e->font, "a", e->fontSize, 0).x;
    }

    { // update editor scroll offset
      // NOTE: do not use old e->row

        const int winWidth = GetScreenWidth();
        const int winHeight = GetScreenHeight();

        // X offset calculation
        const Line line = e->lines.items[e->row]; 
        const int col = e->cx - line.start;
        const int cursorX = col * e->charWidth;
        const int winRight = winWidth - e->scrollX;
        const int winLeft = 0 - e->scrollX;

        if ( cursorX > winRight )
            e->scrollX = winWidth-cursorX-1;
        else if ( cursorX < winLeft )
            e->scrollX = -cursorX;

        // Y offset calulation
        const int cursorTop = e->row* e->fontSize;
        const int cursorBottom = cursorTop + e->fontSize;
        const int winBottom = winHeight - e->scrollY;
        const int winTop = 0 - e->scrollY;

        if (cursorBottom > winBottom)
            e->scrollY = winHeight - cursorBottom;
        else if (cursorTop < winTop)
            e->scrollY = -cursorTop;
    }
}

void draw(Editor *e)
{
        BeginDrawing();
        ClearBackground(BLACK);

        { // Rendering text from buffer
            for (size_t i=0; i<e->lines.count; i++)
            {
                Line line = e->lines.items[i];

                const char* lineStart = e->buffer.items + line.start;
                const size_t lineSize = line.end - line.start;
                char *text = malloc(lineSize + 1);
                memcpy(text, lineStart, lineSize);
                text[lineSize] = '\0';
                Vector2 pos = (Vector2) {
                    0+e->scrollX,
                    (int)(e->fontSize*i) + e->scrollY,
                    // NOTE: i being size_t causes HUGE(obviously) underflow on line 0 when scrollY < 0
                    // -  solution cast to (int): may cause issue later (pain) :( 
                };

                DrawTextEx( e->font, text, pos, e->fontSize, 0, GREEN);
                free(text);
            }
        }
        
        { // Rendering cursor (atleast trying to)
            Line line = e->lines.items[e->row];
            size_t col = e->cx - line.start;

            int cursorX = col * e->charWidth + e->scrollX;
            int cursorY = e->row * e->fontSize + e->scrollY;

            DrawLine(cursorX+1, cursorY, cursorX+1, cursorY + e->fontSize, PINK);
        }

        EndDrawing();
}

int main(int argc, char **argv)
{
    InitWindow(800, 600, "the bingchillin text editor");
    SetTargetFPS(60);
    /*SetTraceLogLevel(LOG_DEBUG);*/

    Editor editor = {0};

    editor_init(&editor);

    if (argc > 1) {
        editor_load_file(&editor, argv[1]);
    }
    
    while(!WindowShouldClose())
    {
        update(&editor);
        draw(&editor);
    }

    editor_deinit(&editor);
    CloseWindow();

    return 0;
}
