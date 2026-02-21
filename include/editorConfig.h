#include <termios.h>
#include <time.h>
#include "rows.h"
#ifndef EC_H
#define EC_H

struct editorConfig {
	int cx;
	int cy;
	int rx;
	int rowOff;
	int scrollOff;
	int coloff;
	int screenRows;
	int screenCols;
	int numRows;
	char* filename;
	erow *row;
	char statusMessage[80];
	time_t statusmsg_time;
	struct termios orig_termios;
};

extern struct editorConfig E;

#endif

