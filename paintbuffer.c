#include <string.h>
#include <stdlib.h>

#include "paintbuffer.h"
#include "tops.h"

Doubly_Linked_List paint_buffer;
size_t paint_buffer_size = 0;

void ClearPaintBuffer()
{

    for (Doubly_Linked_List_Node *node = paint_buffer.head; node != NULL; node = node->next)
    {
        free(node->object);
    }

    DoublyLinkedListClear(&paint_buffer);

    paint_buffer_size = 0;
}

void PaintBufferPrint(char *string)
{
    int length = strlen(string);
    char *paint = malloc(length + 1);
    memcpy(paint, string, length + 1);
    DoublyLinkedListAddObjectToEnd(&paint_buffer, paint);
    paint_buffer_size += strlen(paint);
}

void PaintBufferCursorTo(int row, int col)
{
    char format[] = "\e[%i;%iH";
    int formatted_length = snprintf(NULL, 0, format, row, col);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, row, col);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}

ssize_t RenderPaintBuffer(char **buffer)
{
    *buffer = malloc(paint_buffer_size + 1);
    char *tmp = *buffer;
    for (Doubly_Linked_List_Node *node = paint_buffer.head; node != NULL; node = node->next)
    {
        size_t size = strlen(node->object);
        memcpy(tmp, node->object, size);
        tmp += size;
    }
    return paint_buffer_size + 1;
}

void PaintBufferClearRemainingLine()
{
    PaintBufferPrint("\e[0;K");
}

void PaintBufferCursorLeft(int amount)
{
    char format[] = "\e[%iD";
    int formatted_length = snprintf(NULL, 0, format, amount);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, amount);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}

void PaintBufferCursorRight(int amount)
{
    char format[] = "\e[%iC";
    int formatted_length = snprintf(NULL, 0, format, amount);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, amount);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}

void PaintBufferCursorUp(int amount)
{
    char format[] = "\e[%iA";
    int formatted_length = snprintf(NULL, 0, format, amount);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, amount);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}

void PaintBufferCursorDown(int amount)
{
    char format[] = "\e[%iB";
    int formatted_length = snprintf(NULL, 0, format, amount);
    char *formatted_string = malloc(formatted_length + 1);
    snprintf(formatted_string, formatted_length + 1, format, amount);
    PaintBufferPrint(formatted_string);
    free(formatted_string);
}