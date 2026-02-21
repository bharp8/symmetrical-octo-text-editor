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
void editorUpdateRow(erow *row)
{
	int tabs = 0;
	for(int i = 0; i < row->size; i++){
		if(row->chars[i]=='\t')
			tabs++;
	}
	free(row->render);
	row->render = malloc(row->size + tabs*5 + 1);

	int idx = 0;
	for(int i = 0; i < row->size; i++){
		if(row->chars[i]=='\t'){
			row->render[idx++] = ' ';
			while(idx%6 != 0) row->render[idx++] = ' ';
		}
		else
			row->render[idx++] = row->chars[i];
	}
	row->render[idx] = '\0';
	row->rsize = idx;
}

void editorAppendRow(char *s, size_t len)
{
	E.row = realloc(E.row, sizeof(erow)*(E.numRows+1));

	int at = E.numRows;
	E.row[at].size = len;
	E.row[at].chars = malloc(len+1);
	memcpy(E.row[at].chars, s, len);
	E.row[at].chars[len] = '\0';
	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	editorUpdateRow(&E.row[at]);

	E.numRows++;
}

void abFree(struct abuf* ab)
{
	free(ab->b);
}


