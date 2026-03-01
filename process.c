#include <unistd.h>
#include "fileIO.h"
#include "draw.h"
#include "rows.h"
#include "editorConfig.h"
#include "errors.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>


void insertChar(erow *row, int at, int c);
//moves cursor. handles all functionality related to that. Must be called in processKeypress


void editorRowDelChar(erow* row, int at)
{
	if(at < 0 || at >= row->size)return;
	memmove(&row->chars[at], &row->chars[at+1], row->size - at);
	row->size--;
	editorUpdateRow(row);
}
void editorDelChar(void)
{
	if(E.cy == E.numRows) return ;
	erow* row = &E.row[E.cy];
	if(E.cx >= 0){
		editorRowDelChar(row, E.cx);
	}
}
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

void editorInsertChar(int c)
{
	if(E.cy == E.numRows){
		editorAppendRow("", 0 );
	}
	insertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
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

void insertChar(erow *row, int at, int c)
{
	if(at < 0 || at > row->size) at = row->size;
	row->chars = realloc(row->chars, row->size + 2);
	memmove(&row->chars[at +1], &row->chars[at],row->size - at + 1);
	row->size++;
	row->chars[at] = c;
	editorUpdateRow(row);
}


void insertModeProcess(void)
{
	E.mode = INSERT;
	editorRefreshScreen();
	char c;
	while(E.mode == INSERT){
		c = editorReadKey();
		switch(c){
			case '\033':
				E.mode = NORMAL;
				break;
			case '\r':
				/** todo */
				break;
			case '\b':
				editorDelChar();
				break;
			default:
				editorInsertChar(c);
				editorRefreshScreen();
		}
	}
}

void visualModeProcess(void)
{
	die("ERROR: FEATURE  NOT IMPLEMENTED");
}

//Processes keypress. to add functionality, do it here.
void normalModeProcess(void)
{
	E.mode = NORMAL;
	editorRefreshScreen();

	char c = editorReadKey();
	switch(c){
		case 'q':
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
		case 's':
			editorSave();
		case 'h':
		case 'j':
		case 'k':
		case 'l':
			editorMoveCursor(c);
			break;
		case 'x':
			editorDelChar();
			break;
		case 'o':
			E.cy++;
			E.cx = 0;
			insertModeProcess();
			break;
		case 'a':
			E.cx++;
		case 'i':
			insertModeProcess();
			break;
	}
}



