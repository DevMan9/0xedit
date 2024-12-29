#ifndef paintbuffer_h
#define paintbuffer_h

#include "stdio.h"
#include "doublylinkedlist.h"

extern Doubly_Linked_List paint_buffer;
extern size_t paint_buffer_size;

void ClearPaintBuffer();

void PaintBufferCursorTo(int row, int col);

void PaintBufferCursorLeft(int amount);

void PaintBufferCursorRight(int amount);

void PaintBufferCursorUp(int amount);

void PaintBufferCursorDown(int amount);

void PaintBufferPrint(char *string);

ssize_t RenderPaintBuffer(char **buffer);

void PaintBufferClearRemainingLine();

#endif /* paintbuffer_h */