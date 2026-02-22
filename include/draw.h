#ifndef DRAW_H
#define DRAW_H
#include "rows.h"
int getWindowSize(int *rows, int *cols);
void editorDrawRows(struct abuf *ab);
void editorRefreshScreen(void);
void editorSetStatusMessage(const char* fmt, ...);
#endif
