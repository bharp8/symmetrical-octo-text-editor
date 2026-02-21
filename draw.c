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

void editorDrawStatus(struct abuf *ab)
{
	abAppend(ab,"\x1b[7m",4);//inverts colors of bar
	char status[80], rstatus[80];
	int len = snprintf(status, sizeof(status), "%.20s - %d lines", E.filename ? E.filename : "[No Name]", E.numRows);
	int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",E.cy+1, E.numRows);
	if(len > E.screenCols) len = E.screenCols;
	abAppend(ab, status, len);
	while(len< E.screenCols){
		if(E.screenCols - len == rlen){
			abAppend(ab,rstatus,rlen);
			break;
		}
		else {
			abAppend(ab, " ", 1);
			len++;
		}
	}
	abAppend(ab, "\x1b[m", 3);//uninverts
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
			int len = E.row[fileRow].rsize - E.coloff;
			if(len < 0) len = 0;
			if(len > E.screenCols) len = E.screenCols;
			abAppend(ab, &E.row[fileRow].render[E.coloff], len);
		}

		abAppend(ab, "\x1b[K", 3);
		abAppend(ab, "\r\n", 2);
	}
}

int editorRowCxToRx(erow *row, int cx)
{
	int rx = 0;
	for(int i = 0; i < cx; i++){
		if(row->chars[i] == '\t'){
			rx += (5 -1) - (rx % 5);
		}
		rx++;
	}
	return rx;
}

//scrolling functionality.
void editorScroll(void)
{
	E.rx = 0;
	if(E.cy < E.numRows){
		E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
	}

	if(E.cy < E.rowOff){
		E.rowOff = E.cy;
	}
	if(E.cy >= E.rowOff + E.screenRows){
		E.rowOff = E.cy - E.screenRows + 1;
	}
	if(E.rx < E.coloff){
		E.coloff = E.rx;
	}
	if(E.rx >= E.coloff + E.screenCols){
		E.coloff = E.rx - E.screenCols + 1;
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
	editorDrawStatus(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy - E.rowOff +1, E.rx - E.coloff+1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}



