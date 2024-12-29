#ifndef editor_h
#define editor_h

#include "paintbuffer.h"
#include "render.h"
#include "input.h"

#define DEBUG 0b00000001
#define WINDOW_RESIZED 0b00000010

#define DEBUG_LINES 7
#define FOOTER_LINES 2
#define SAVE_LINES 1
#define HEADER_LINES 1
#define DATA_INDENT 1

#define HEXADECIMAL_INDICES_PER_BYTE 2
#define BINARY_INDICES_PER_BYTE 8
#define QUATERNARY_INDICES_PER_BYTE 4
#define ASCII_INDICES_PER_BYTE 1

#define DEFAULT_STATE EDIT
#define DEFAULT_MODE HEXADECIMAL
#define DEFAULT_CURSOR_INDEX 0
#define DEFAULT_DATA_DISPLAY_INDEX 0
#define DEFAULT_GROUPS_PER_LINE 8
#define DEFAULT_BYTES_PER_GROUP HEXADECIMAL_INDICES_PER_BYTE

typedef struct Tuple_S
{
    int y;
    int x;
} Tuple;

typedef struct Window_S
{
    Tuple root;
    Tuple dimensions;
} Window;

typedef enum Mode_E
{
    BINARY,
    QUATERNARY,
    OCTAL,
    HEXADECIMAL,
    ASCII
} Mode;

typedef enum State_S
{
    EDIT,
    SAVE
} State;

typedef struct Editor_Data_S
{
    // Stores the previously submitted input
    char input[17];

    // For testing purposes, delete later.
    char raw_data_array[256];
    // A Doubly_Linked_List* of Doubly_Linked_List* of char* storing the current working data.
    Doubly_Linked_List *raw_data;
    // The dimensions of the window.
    Tuple window;

    // The state of the editor
    State state;

    // Additional flags
    unsigned int flags;

    // The mode that the editor is working in.
    Mode mode;

    // The index of the cursor.
    ssize_t cursor_index;

    // The index to start displaying data from.
    ssize_t data_display_index;

    // The number of characters used to display a byte.
    // Derived from mode.
    unsigned char indices_per_byte;

    // The number of bits per index.
    // Derived from mode.
    size_t bits_per_index;

    // The number of bytes to be displayed in a group.
    // Derived from None.
    // Limited by window.x.
    unsigned char bytes_per_group;
    // The number of groups to be displayed on a line.
    // Derived from None.
    // Limited by window.x.
    unsigned char groups_per_line;
    // The number of bytes that will be displayed on a line.
    // Derived from groups_per_line, bytes_per_group, and indices_per_byte.
    unsigned short indices_per_line;

    // Deprecate
    unsigned char header_rows;
    // Deprecate
    unsigned char footer_rows;

    // The number of lines that the window is capable of displaying.
    // Derived from window.y, header_rows, and footer_rows.
    unsigned char lines_per_page;
    // The number of indices per page.
    // Derived from lines_per_page and indices_per_line.
    size_t indices_per_page;
    // The screen position to begin displaying the data from.
    // Derived from header_rows.
    Tuple data_display_root;

    Window data_window;
    Window debug_window;
    Window footer_window;
    Window header_window;
    Window save_window;

} Editor_Data;

extern Editor_Data editor_data;

void Initialize();

void ChangeMode(Mode mode);

void ChangeState(State state);

void UpdateDerivatives();

void ToggleDebug();

void CursorIndexToRowAndColumn(int *row, int *col);

void WindowResized();

#endif /* editor_h */