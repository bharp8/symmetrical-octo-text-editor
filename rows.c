#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include "editorConfig.h"
#include "rows.h"





void abAppend(struct abuf *ab, const char* s, int len)
{
	char *new = realloc(ab->b, ab->len + len);

	if (new == NULL) return;
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void editorAppendRow(char *s, size_t len)
{
	E.row = realloc(E.row, sizeof(erow)*(E.numRows+1));

	int at = E.numRows;
	E.row[at].size = len;
	E.row[at].chars = malloc(len+1);
	memcpy(E.row[at].chars, s, len);
	E.row[at].chars[len] = '\0';
	E.numRows++;

}

void abFree(struct abuf* ab)
{
	free(ab->b);
}


