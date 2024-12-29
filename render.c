#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "render.h"
#include "editor.h"

const char HEX_MAP[16] = "0123456789ABCDEF";

void PaintStandard()
{
    size_t buffer_size = editor_data.indices_per_line + editor_data.groups_per_line;
    char *buffer = malloc(buffer_size);
    buffer[buffer_size - 1] = '\0';

    size_t buffer_index = 0;
    size_t sub_index = 0;
    size_t byte_index = 0;
    size_t group_index = 0;
    size_t line_index = 0;

    for (size_t read_sub_index = 0; read_sub_index < editor_data.indices_per_page; read_sub_index++)
    {
        size_t raw_sub_index = read_sub_index + editor_data.data_display_index;
        size_t raw_index = raw_sub_index / editor_data.indices_per_byte;
        unsigned char mask = ~(~0 << editor_data.bits_per_index);
        unsigned char shift = (editor_data.indices_per_byte - (raw_sub_index % editor_data.indices_per_byte) - 1) * editor_data.bits_per_index;

        buffer[buffer_index] = HEX_MAP[(editor_data.raw_data_array[raw_index] >> shift) & mask];

        buffer_index++;
        sub_index++;
        if (sub_index >= editor_data.indices_per_byte)
        {
            sub_index = 0;
            byte_index++;
        }
        if (byte_index >= editor_data.bytes_per_group)
        {
            byte_index = 0;
            buffer[buffer_index] = ' ';
            buffer_index++;
            group_index++;
        }
        if (group_index >= editor_data.groups_per_line)
        {
            PaintBufferCursorTo(editor_data.data_display_root.y + line_index, editor_data.data_display_root.x);
            PaintBufferPrint(buffer);
            PaintBufferClearRemainingLine();

            group_index = 0;
            buffer_index = 0;
            line_index++;
        }
    }
}

void PaintOctal()
{
    PaintBufferCursorTo(editor_data.data_display_root.y, editor_data.data_display_root.x);
    PaintBufferPrint("[OCTAL not implemented]");
}

void PaintASCII()
{
    PaintBufferCursorTo(editor_data.data_display_root.y, editor_data.data_display_root.x);
    PaintBufferPrint("[ASCII not implemented]");
}

void PaintData()
{
    switch (editor_data.mode)
    {
    case BINARY:
        PaintStandard();
        break;
    case QUATERNARY:
        PaintStandard();
        break;
    case OCTAL:
        PaintOctal();
        break;
    case HEXADECIMAL:
        PaintStandard();
        break;
    case ASCII:
        PaintASCII();
        break;

    default:
        break;
    }
}

void PaintDebug()
{
    char *format;
    int formatted_length;
    char *formatted_string;

    PaintBufferCursorTo(editor_data.debug_window.root.y, editor_data.debug_window.root.x);
    PaintBufferPrint("[DEBUG]");
    PaintBufferClearRemainingLine();
    PaintBufferCursorTo(editor_data.debug_window.root.y + 1, editor_data.debug_window.root.x);

    format = "cursor_index: %zu   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.cursor_index);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.cursor_index);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    format = "data_display_index: %zu   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.data_display_index);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.data_display_index);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    PaintBufferClearRemainingLine();
    PaintBufferCursorTo(editor_data.debug_window.root.y + 2, editor_data.debug_window.root.x);

    format = "window: {rows: %i, columns %i}   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.window.y, editor_data.window.x);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.window.y, editor_data.window.x);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    PaintBufferClearRemainingLine();
    PaintBufferCursorTo(editor_data.debug_window.root.y + 3, editor_data.debug_window.root.x);

    format = "indices_per_byte: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.indices_per_byte);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.indices_per_byte);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    format = "bytes_per_group: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.bytes_per_group);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.bytes_per_group);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    format = "groups_per_line: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.groups_per_line);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.groups_per_line);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    PaintBufferClearRemainingLine();
    PaintBufferCursorTo(editor_data.debug_window.root.y + 4, editor_data.debug_window.root.x);

    format = "indices_per_line: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.indices_per_line);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.indices_per_line);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    format = "lines_per_page: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.lines_per_page);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.lines_per_page);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    format = "indices_per_page: %i   ";
    formatted_length = snprintf(NULL, 0, format, editor_data.indices_per_page);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, editor_data.indices_per_page);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    PaintBufferClearRemainingLine();

    PaintBufferCursorTo(editor_data.debug_window.root.y + 5, editor_data.debug_window.root.x);

    format = "flags: %s   ";

    char bin[33];
    bin[32] = '\0';

    for (unsigned char i = 0; i < 32; i++)
    {
        bin[31 - i] = ((editor_data.flags >> i) & 1) ? '1' : '0';
    }

    formatted_length = snprintf(NULL, 0, format, bin);
    formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, bin);
    PaintBufferPrint(formatted_string);
    free(formatted_string);

    PaintBufferClearRemainingLine();

    PaintBufferCursorTo(editor_data.debug_window.root.y + 6, editor_data.debug_window.root.x);

    char *printput = malloc(strlen(editor_data.input));
    memcpy(printput, editor_data.input, strlen(editor_data.input) + 1);
    for (int i = 0; i < strlen(printput); i++)
    {
        printput[i] = isprint(printput[i]) ? printput[i] : '_';
    }
    PaintBufferPrint("Last input: ");
    PaintBufferPrint(printput);
    PaintBufferClearRemainingLine();
    free(printput);

    PaintBufferClearRemainingLine();
}

void PaintMode()
{
    char format[] = "[Mode: %s]";
    char *mode;
    switch (editor_data.mode)
    {
    case BINARY:
        mode = "Binary";
        break;
    case QUATERNARY:
        mode = "Quaternary";
        break;
    case OCTAL:
        mode = "Octal";
        break;
    case HEXADECIMAL:
        mode = "Hexadecimal";
        break;
    case ASCII:
        mode = "ASCII";
        break;

    default:
        mode = "!!!UNKNOWN!!!";
        break;
    }

    int formatted_length = snprintf(NULL, 0, format, mode);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, mode);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}

void PaintFooter()
{
    for (int i = 0; i < editor_data.footer_window.dimensions.y; i++)
    {
        PaintBufferCursorTo(editor_data.footer_window.root.y + i, editor_data.footer_window.root.x);

        char format[] = "[Footer line %i]";
        int formatted_length = snprintf(NULL, 0, format, i);
        char *formatted_string = malloc(formatted_length + 1);
        snprintf(formatted_string, formatted_length + 1, format, i);
        PaintBufferPrint(formatted_string);
        free(formatted_string);

        PaintBufferClearRemainingLine();
    }
}

void PaintSavePrompt()
{
    for (int i = 0; i < editor_data.save_window.dimensions.y; i++)
    {
        PaintBufferCursorTo(editor_data.save_window.root.y + i, editor_data.save_window.root.x);

        char format[] = "[Save prompt line %i]";
        int formatted_length = snprintf(NULL, 0, format, i);
        char *formatted_string = malloc(formatted_length + 1);
        snprintf(formatted_string, formatted_length + 1, format, i);
        PaintBufferPrint(formatted_string);
        free(formatted_string);

        PaintBufferClearRemainingLine();
    }
}

void DisplayScreen()
{
    PaintBufferCursorTo(1, 1);
    PaintBufferClearRemainingLine();
    PaintBufferPrint("---0xedit: Hex Editor---");
    PaintBufferCursorRight(3);
    PaintMode();

    PaintData();

    if (editor_data.flags & DEBUG)
    {
        PaintDebug();
    }

    if (editor_data.state == SAVE)
    {
        PaintSavePrompt();
    }

    PaintFooter();

    int row = 0;
    int col = 0;

    CursorIndexToRowAndColumn(&row, &col);
    PaintBufferCursorTo(row, col);

    char *render_buffer;
    ssize_t size = RenderPaintBuffer(&render_buffer);
    write(STDOUT_FILENO, render_buffer, size);
    free(render_buffer);

    ClearPaintBuffer();
}