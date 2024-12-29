#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tops.h"

int GetCursorPosition2(int *row, int *col)
{
    int ret = 0;
    char stdin_buffer[LINE_MAX];
    ssize_t stdin_size = 0;

    // Request cursor position
    if (write(STDOUT_FILENO, "\e[6n", 4) != 4)
    {
        ret = -1;
        goto end;
    }

    // Read all of stdin
    stdin_size = read(STDIN_FILENO, stdin_buffer, LINE_MAX);

    // Find and remove "\e[%i;%iR"
    if (sscanf(stdin_buffer, "\e[%i;%iR", row, col) != 2)
    {
        ret = -1;
        goto end;
    }

end:
    // Put stripped data back into stdin
    if (stdin_size > 0)
    {
        write(STDIN_FILENO, stdin_buffer, stdin_size);
    }
    return ret;
}

// Rewrite this completely
int GetCursorPosition(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\e[6n", 4) != 4)
    {
        return -1;
    }

    while (i < sizeof(buf) - 1)
    {
        // TODO This fails if 2 keys are pressed in quick succession
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
        {
            break;
        }

        if (buf[i] == 'R')
        {
            break;
        }

        i++;
    }
    buf[i + 1] = '\0';

    for (char *root = strstr(buf, "\e["); root != NULL; root = strstr(root + 1, "\e["))
    {
        if (sscanf(root, "\e[%d;%dR", rows, cols) != 2)
        {
            return 0;
        }
    }

    return -1;
}

int GetWindowSize(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        int row = -1;
        int col = -1;
        GetCursorPosition(&row, &col);

        if (CursorTo(1024, 1024))
        {
            return -1;
        }
        if (GetCursorPosition2(rows, cols))
        {
            return -1;
        }
        if (CursorTo(row, col))
        {
            return -1;
        }
        return 0;
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

int CursorTo(int row, int col)
{
    char format[] = "\e[%i;%iH";
    int formatted_length = snprintf(NULL, 0, format, row, col);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, row, col);
    if (write(STDOUT_FILENO, formatted_string, formatted_length) != formatted_length)
    {
        return -1;
    }
    free(formatted_string);
    return 0;
}