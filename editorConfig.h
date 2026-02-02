#include <termios.h>
#include "rows.h"
#ifndef EC_H
#define EC_H
struct editorConfig {
	int cx;
	int cy;
	int rowOff;
	int screenRows;
	int screenCols;
	int numRows;
	erow *row;
	struct termios orig_termios;
};

extern struct editorConfig E;

#endif
