#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

#define CTRL_KEY(k) (k & 0x1f)

int HandleControl(char **input)
{
    char *c = *input;
    switch (*c)
    {
    case CTRL_KEY('a'):
        ChangeMode(ASCII);
        break;
    case CTRL_KEY('b'):
        ChangeMode(BINARY);
        break;
    case CTRL_KEY('f'):
        ChangeMode(QUATERNARY);
        break;
    case CTRL_KEY('h'):
        ChangeMode(HEXADECIMAL);
        break;
    case CTRL_KEY('o'):
        ChangeMode(OCTAL);
        break;
    case CTRL_KEY('d'):
        ToggleDebug();
        break;
    case CTRL_KEY('s'):
        ChangeState((editor_data.state == EDIT) ? SAVE : EDIT);
        break;

    case '\e':
    {
        if (strstr(c, "\e[A"))
        {
            editor_data.cursor_index -= editor_data.indices_per_line;
            editor_data.data_display_index = editor_data.data_display_index - ((editor_data.cursor_index < editor_data.data_display_index) ? editor_data.indices_per_line : 0);
            c += 2;
        }
        else if (strstr(c, "\e[B"))
        {
            editor_data.cursor_index += editor_data.indices_per_line;
            editor_data.data_display_index = editor_data.data_display_index + ((editor_data.cursor_index >= editor_data.data_display_index + editor_data.indices_per_page) ? editor_data.indices_per_line : 0);
            c += 2;
        }
        else if (strstr(c, "\e[C"))
        {
            editor_data.cursor_index++;
            editor_data.data_display_index = editor_data.data_display_index + ((editor_data.cursor_index >= editor_data.data_display_index + editor_data.indices_per_page) ? editor_data.indices_per_line : 0);
            c += 2;
        }
        else if (strstr(c, "\e[D"))
        {
            editor_data.cursor_index--;
            editor_data.data_display_index = editor_data.data_display_index - ((editor_data.cursor_index < editor_data.data_display_index) ? editor_data.indices_per_line : 0);
            c += 2;
        }
        else if (strstr(c, "\e[1;2A"))
        {
            editor_data.bytes_per_group++;
            UpdateDerivatives();
            c += 5;
        }
        else if (strstr(c, "\e[1;2B"))
        {
            editor_data.bytes_per_group = editor_data.bytes_per_group - ((editor_data.bytes_per_group > 1) ? 1 : 0);
            UpdateDerivatives();
            c += 5;
        }
        else if (strstr(c, "\e[1;2C"))
        {
            editor_data.groups_per_line++;
            UpdateDerivatives();
            c += 5;
        }
        else if (strstr(c, "\e[1;2D"))
        {
            editor_data.groups_per_line = editor_data.groups_per_line - ((editor_data.groups_per_line > 1) ? 1 : 0);
            UpdateDerivatives();
            c += 5;
        }
    }
    break;

    default:
        break;
    }
    *input = c;
    return 0;
}

int HandleBinary(char *c)
{
    size_t raw_data_index = editor_data.cursor_index / editor_data.indices_per_byte;
    unsigned char bit = editor_data.cursor_index % editor_data.indices_per_byte;
    unsigned char shift = (7 - bit);
    unsigned char mask = ~(~0U << 1) << shift;
    unsigned char input = 0;

    switch (*c)
    {
    case '0':
    case '1':
        input = *c == '1';
        goto valid;

    valid:
        editor_data.raw_data_array[raw_data_index] = (editor_data.raw_data_array[raw_data_index] & ~mask) | (input << shift);
        editor_data.cursor_index++;
        return 0;

    default:
        return -1;
    }
}

int HandleQuaternary(char *c)
{
    size_t raw_data_index = editor_data.cursor_index / editor_data.indices_per_byte;
    unsigned char index = editor_data.cursor_index % editor_data.indices_per_byte;
    unsigned char shift = (3 - index) * editor_data.bits_per_index;
    unsigned char mask = ~(~0U << editor_data.bits_per_index) << shift;
    unsigned char input = 0;

    switch (*c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
        input = *c - '0';
        goto valid;

    valid:
        editor_data.raw_data_array[raw_data_index] = (editor_data.raw_data_array[raw_data_index] & ~mask) | (input << shift);
        editor_data.cursor_index++;
        return 0;

    default:
        return -1;
    }
}

int HandleOctal(char *c)
{
    return 0;
}

int HandleHexadecimal(char *c)
{
    size_t raw_data_index = editor_data.cursor_index / editor_data.indices_per_byte;
    int half = editor_data.cursor_index & 1;
    char mask = (half) ? ~0xf : 0xf;
    char input = 0;
    switch (*c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        input = *c - '0';
        goto valid;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        input = *c - 'a' + 10;
        goto valid;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        input = *c - 'A' + 10;
        goto valid;

    valid:
        input = input << ((half) ? 0 : 4);
        editor_data.raw_data_array[raw_data_index] = (editor_data.raw_data_array[raw_data_index] & (mask)) | input;
        editor_data.cursor_index++;
        return 0;
    default:
        return -1;
    }
}

int HandleASCII(char *c)
{
    return 0;
}

// These need to be changed when ASCII is implemented.
int HandleCharacterOperations(char *c)
{
    switch (*c)
    {
    case ',':
    case '<':
    {
        editor_data.cursor_index--;
        editor_data.data_display_index--;
    }
    break;
    case '.':
    case '>':
    {
        editor_data.cursor_index++;
        editor_data.data_display_index++;
    }
    break;
    default:
        break;
    }
    return 0;
}

void HandleInput(char *input)
{
    for (char *c = input; *c != '\0'; c++)
    {
        if (iscntrl(*c))
        {
            if (HandleControl(&c))
            {
                break;
            }
        }
        else
        {
            switch (editor_data.mode)
            {
            case BINARY:
                HandleBinary(c);
                break;
            case QUATERNARY:
                HandleQuaternary(c);
                break;
            case OCTAL:
                HandleOctal(c);
                break;
            case HEXADECIMAL:
                HandleHexadecimal(c);
                break;
            case ASCII:
                HandleASCII(c);
            default:
                break;
            }

            HandleCharacterOperations(c);
        }
        if (*c == CTRL_KEY('q') || *c == CTRL_KEY('x'))
        {
            exit(0);
        }
    }
}