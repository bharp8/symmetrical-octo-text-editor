#include <stddef.h>
#ifndef ROWS_H
#define ROWS_H

typedef struct erow {
	int size;
	char* chars;
} erow;

struct abuf {
	char* b;
	int len;
};

#define ABUF_INIT {NULL, 0}


void abAppend(struct abuf *ab, const char* s, int len);
void editorAppendRow(char *s, size_t len);
void abFree(struct abuf* ab);
#endif
