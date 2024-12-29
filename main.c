#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "editor.h"
#include "paintbuffer.h"

struct termios orig_termios;
struct termios current_termios;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
        die("tcsetattr");
    }
}

void enableRawMode()
{
    // store the current termios settings for STDIN_FILENO in orig_termios
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        die("tcgetattr");
    }

    // bind the disable raw mode function to exit().
    atexit(disableRawMode);

    // Initialize a copy of the original termios settings that we will convert to raw mode
    current_termios = orig_termios;

    current_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    current_termios.c_oflag &= ~(OPOST);
    current_termios.c_cflag |= (CS8);
    current_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    current_termios.c_cc[VMIN] = 0;
    current_termios.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &current_termios);
}

int main(int argc, char **argv)
{
    printf("0xedit\n");

    enableRawMode();
    Initialize();
    PaintBufferPrint("\e[2J");
    DisplayScreen();

    while (1)
    {
        int status = read(STDIN_FILENO, editor_data.input, 16);
        if (status == -1 && errno != EAGAIN && errno != EINTR)
        {
            die("read");
        }

        if (status > 0)
        {
            editor_data.input[status] = '\0';
            HandleInput(editor_data.input);
            DisplayScreen();
        }
        else if (editor_data.flags & WINDOW_RESIZED)
        {
            WindowResized();
            DisplayScreen();
        }
    }

    return 0;
}