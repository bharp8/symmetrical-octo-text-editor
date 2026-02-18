#include "editorConfig.h"
#include "draw.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//gets window size. Probably shouldn't be changed, unless to add windows functionality.
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

//IP. Draws all rows.
void editorDrawRows(struct abuf *ab)
{

	for(int y = 0; y < E.screenRows; y++){
		int fileRow = y + E.rowOff;
		if(fileRow >= E.numRows){
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
			int len = E.row[fileRow].size - E.coloff;
			if(len < 0) len = 0;
			if(len > E.screenCols) len = E.screenCols;
			abAppend(ab, &E.row[fileRow].chars[E.coloff], len);
		}

		abAppend(ab, "\x1b[K", 3);
		if(y<E.screenRows -1 ){
			abAppend(ab, "\r\n", 2);
		}
	}
}

//scrolling functionality.
void editorScroll(void)
{
	if(E.cx < E.rowOff){
		E.rowOff = E.cx;
	}
	if(E.cx >= E.rowOff + E.screenRows){
		E.rowOff = E.cx - E.screenRows + 1;
	}
	if(E.cy < E.coloff){
		E.coloff = E.cy;
	}
	if(E.cy >= E.coloff + E.screenCols){
		E.coloff = E.cy - E.screenCols + 1;
	}
}

//refreshes screen. In progress.
void editorRefreshScreen(void)
{
	editorScroll();
	struct abuf ab = ABUF_INIT;


	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	editorDrawRows(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy - E.rowOff +1, E.cx+1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}



