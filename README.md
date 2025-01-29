# the bingchillin text editor

Just writing this for fun in C with raylib for rendering.

![image](screenshot.png)

## Controls


|Key             |Action                         |
|----------------|-------------------------------|
|Ctrl =          |font size increase             |
|Ctrl -          |font size decrease             |
|Right Arrow     |Cursor right                   |
|Left Arrow      |Cursor left                    |
|Up Arrow        |Cursor up                      |
|Down Arrow      |Cursor down                    |
|Backspace       |remove character before cursor |
|Delete          |remove character at cursor     |
|Ctrl Backspace  |remove word before cursor      |
|Ctrl Delete     |remove word after cursor       |
|Enter           |insert a newline               |
|Home            |Cursor to line beginning       |
|End             |Cursor to line end             |
|Ctrl Home       |Cursor to file beginning       |
|Ctrl End        |Cursor to file end             |
|Ctrl Up         |Cursor to previous empty line  |
|Ctrl Down       |Cursor to next empty line      |
|PageUp          |Cursor moves 10 lines up       |
|PageDown        |Cursor moves 10 lines down     |
|Shift \[movement]|Select stuff on cursor movement|
|Backspace/Delete|Deletes selection              |
|Escape          |Clears selection               |
|Ctrl A          |Select all                     |
|Ctrl C          |Copy selection or current line |
|Ctrl V          |Paste into editor              |

## TODO

- [x] display line numbers
- [x] proper cursor position calulation on font size change
- [x] un-jankify the rendering (please.)
  - done....?
- [x] implement text selection and basic selection operations
  - very rudimentary selection with much bugs
- [x] refine selection stuff - proper width and position calculation for font size
  change
- [x] creating new file when given file doesnt exist
- [x] basic notification system for giving user feedback (e.g. Saved to filename *XYZ*
  )
- [x] copy paste functionality
- [ ] when saving: append `\n` char to end of buffer if no `\n` exists as the last
  character
