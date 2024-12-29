#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "editor.h"
#include "tops.h"

Editor_Data editor_data;

void ResizeHandler(int signo)
{
    editor_data.flags |= WINDOW_RESIZED;
}

void UpdateWindows()
{
    editor_data.header_window.dimensions.x = editor_data.window.x;
    editor_data.header_window.dimensions.y = HEADER_LINES;
    editor_data.header_window.root.x = 1;
    editor_data.header_window.root.y = 1;

    if (editor_data.flags & DEBUG)
    {
        editor_data.debug_window.dimensions.x = editor_data.window.x;
        editor_data.debug_window.dimensions.y = DEBUG_LINES;
    }
    else
    {
        editor_data.debug_window.dimensions.x = 0;
        editor_data.debug_window.dimensions.y = 0;
    }
    editor_data.debug_window.root.x = 1;
    editor_data.debug_window.root.y = 1 + editor_data.window.y - editor_data.debug_window.dimensions.y;

    editor_data.footer_window.dimensions.x = editor_data.window.x;
    editor_data.footer_window.dimensions.y = FOOTER_LINES;
    editor_data.footer_window.root.x = 1;
    editor_data.footer_window.root.y = editor_data.debug_window.root.y - editor_data.footer_window.dimensions.y;

    if (editor_data.state == SAVE)
    {
        editor_data.save_window.dimensions.x = editor_data.window.x;
        editor_data.save_window.dimensions.y = 1;
    }
    else
    {
        editor_data.save_window.dimensions.x = 0;
        editor_data.save_window.dimensions.y = 0;
    }
    editor_data.save_window.root.x = 1;
    editor_data.save_window.root.y = editor_data.footer_window.root.y - editor_data.save_window.dimensions.y;

    editor_data.data_window.dimensions.x = editor_data.window.x;
    editor_data.data_window.dimensions.y = editor_data.save_window.root.y - (editor_data.header_window.root.y + editor_data.header_window.dimensions.y);
    editor_data.data_window.root.x = 1;
    editor_data.data_window.root.y = editor_data.header_window.root.y + editor_data.header_window.dimensions.y;
}

void WindowResized()
{
    GetWindowSize(&editor_data.window.y, &editor_data.window.x);
    UpdateWindows();
    UpdateDerivatives();
    editor_data.flags &= ~WINDOW_RESIZED;
}

void UpdateBitsPerIndex()
{
    editor_data.bits_per_index = 8;
    for (unsigned char indices_per_byte = editor_data.indices_per_byte; indices_per_byte > 1; indices_per_byte = indices_per_byte >> 1)
    {
        editor_data.bits_per_index = editor_data.bits_per_index >> 1;
    }
}

void UpdateDerivatives()
{
    editor_data.footer_rows = FOOTER_LINES + ((editor_data.flags & DEBUG) ? DEBUG_LINES : 0) + ((editor_data.state == SAVE) ? SAVE_LINES : 0);
    editor_data.lines_per_page = editor_data.window.y - editor_data.header_rows - editor_data.footer_rows;
    editor_data.data_display_root.y = 1 + editor_data.header_rows;
    editor_data.indices_per_line = editor_data.bytes_per_group * editor_data.groups_per_line * editor_data.indices_per_byte;
    editor_data.indices_per_page = editor_data.indices_per_line * editor_data.lines_per_page;
    UpdateBitsPerIndex();
    UpdateWindows();
}

void ChangeMode(Mode mode)
{
    editor_data.mode = mode;
    editor_data.cursor_index /= editor_data.indices_per_byte;
    editor_data.data_display_index /= editor_data.indices_per_byte;
    switch (editor_data.mode)
    {
    case BINARY:
        editor_data.indices_per_byte = BINARY_INDICES_PER_BYTE;
        break;
    case QUATERNARY:
        editor_data.indices_per_byte = QUATERNARY_INDICES_PER_BYTE;
        break;
    case OCTAL:
        editor_data.indices_per_byte = 3;
        break;
    case ASCII:
        editor_data.indices_per_byte = ASCII_INDICES_PER_BYTE;
        break;

    case HEXADECIMAL:
    default:
    {
        editor_data.mode = HEXADECIMAL;
        editor_data.indices_per_byte = HEXADECIMAL_INDICES_PER_BYTE;
    }
    break;
    }

    UpdateDerivatives();
    editor_data.cursor_index *= editor_data.indices_per_byte;
}

void ChangeState(State state)
{
    editor_data.state = state;

    UpdateDerivatives();
}

void ToggleDebug()
{
    editor_data.flags ^= DEBUG;

    UpdateDerivatives();
}

void Initialize()
{
    struct sigaction window_resize_action;
    window_resize_action.__sigaction_u.__sa_handler = &ResizeHandler;

    sigaction(SIGWINCH, &window_resize_action, NULL);
    GetWindowSize(&editor_data.window.y, &editor_data.window.x);
    editor_data.input[16] = '\0';
    editor_data.raw_data = CreateDoublyLinkedList();
    editor_data.cursor_index = DEFAULT_CURSOR_INDEX;
    editor_data.data_display_index = DEFAULT_DATA_DISPLAY_INDEX;
    editor_data.bytes_per_group = DEFAULT_BYTES_PER_GROUP;
    editor_data.groups_per_line = DEFAULT_GROUPS_PER_LINE;
    editor_data.header_rows = 1;
    UpdateWindows();
    ChangeState(DEFAULT_STATE);

    ChangeMode(DEFAULT_MODE);

    editor_data.data_display_root.y = editor_data.header_rows + 1;
    editor_data.data_display_root.x = DATA_INDENT;
}

void CursorIndexToRowAndColumn(int *row, int *col)
{
    switch (editor_data.mode)
    {
    case BINARY:
    case QUATERNARY:
    case OCTAL:
    case HEXADECIMAL:
    {
        ssize_t display_index = editor_data.cursor_index - editor_data.data_display_index;
        ssize_t sub_byte = display_index;
        ssize_t byte = sub_byte / editor_data.indices_per_byte;
        ssize_t group = byte / editor_data.bytes_per_group;
        ssize_t line = group / editor_data.groups_per_line;

        sub_byte = sub_byte % editor_data.indices_per_byte;
        byte = byte % editor_data.bytes_per_group;
        group = group % editor_data.groups_per_line;

        *row = editor_data.data_display_root.y + line;

        *col = editor_data.data_display_root.x +
               sub_byte +
               byte * editor_data.indices_per_byte +
               group * (editor_data.bytes_per_group * editor_data.indices_per_byte + 1);
    }
    break;
    case ASCII:
    {
    }
    break;
    default:
        break;
    }
}
