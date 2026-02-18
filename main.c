#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "errors.h"
#include "fileIO.h"
#include "editorConfig.h"
#include "rows.h"
#include "rawMode.h"
#include "draw.h"
#include "process.h"



#define BMH_VERSION "0.0.1"

void initEditor(void)
{
	E.cx = 0;
	E.cy = 0;
	E.numRows = 0;
	E.row = NULL;
	E.coloff = 0;
	E.rowOff = 0;
	E.scrollOff=0;

	if(getWindowSize(&E.screenRows, &E.screenCols)==-1) die("getWindowSize");
}

int main(int argc, char* argv[])
{
	enableRawMode();
	initEditor();
	if(argc >= 2){
		editorOpen(argv[1]);
	}

	char c;
	while(1){
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}
