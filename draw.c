#include "editorConfig.h"
#include "draw.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0){
		return -1;
	}
	else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

void editorDrawRows(struct abuf *ab)
{

	for(int y = 0; y < E.screenRows; y++){
		if(y>=E.numRows){
			if(E.numRows == 0 && y == E.screenRows/3){
				char welcome[80];
				int welcomelen = snprintf(welcome, strlen(welcome), "BMH editor version ", 1);
				if(welcomelen > E.screenCols) welcomelen = E.screenCols;
				int padding = (E.screenCols - welcomelen)/2;
				if(padding){
					abAppend(ab, "~", 1);
					padding --;
				}
				while (padding--) abAppend(ab, " ", 1);
				abAppend(ab, welcome, welcomelen);
			}
			else{
				abAppend(ab, "~", 1);
			}
		}
		else {
			int len = E.row[y].size;
			if(len > E.screenCols) len = E.screenCols;
			abAppend(ab, E.row[y].chars, len);
		}

		abAppend(ab, "\x1b[K", 3);
		if(y<E.screenRows -1 ){
			abAppend(ab, "\r\n", 2);
		}
	}
}

void editorRefreshScreen(void)
{
	struct abuf ab = ABUF_INIT;


	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	editorDrawRows(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy +1, E.cx+1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}



