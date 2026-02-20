#include <unistd.h>
#include "editorConfig.h"
#include "errors.h"
#include <errno.h>
#include <stdlib.h>

//moves cursor. handles all functionality related to that. Must be called in processKeypress
void editorMoveCursor(char key)
{
	erow *row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];
	switch(key){
		case 'h':
			if(E.cx!=0)
				E.cx--;
			break;
		case 'j':
			if(E.cy < E.numRows)
				E.cy++;
			break;
		case 'k':
			if(E.cy != 0)
				E.cy--;
			break;
		case 'l':
			if(row && E.cx < row->size) {
				E.cx++;
			}
			break;

	}
	row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if (E.cx > rowlen){
		E.cx = rowlen;
	}
}

//reads the next key. should probably not be changed.
char editorReadKey(void)
{
	int nread;
	char c;
	while((nread = read(STDIN_FILENO, &c, 1)) !=1){
		if(nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}

//Processes keypress. to add functionality, do it here.
void editorProcessKeypress(void)
{
	char c = editorReadKey();

	switch(c){
		case 'q':
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;

		case 'h':
		case 'j':
		case 'k':
		case 'l':
			editorMoveCursor(c);
			break;
	}
}


