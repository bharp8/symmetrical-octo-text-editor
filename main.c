#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>


#define BMH_VERSION "0.0.1"

struct editorConfig {
	int cx;
	int cy;
	int screenRows;
	int screenCols;
	struct termios orig_termios;
};

struct editorConfig E;

void editorMoveCursor(char key)
{
	switch(key){
		case 'h':
			E.cx--;
			break;
		case 'j':
			E.cy++;
			break;
		case 'k':
			E.cy--;
			break;
		case 'l':
			E.cx++;
			break;
	}
}
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
struct abuf {
	char* b;
	int len;
};

#define ABUF_INIT {NULL, 0}

void disableRawMode(void);

void abAppend(struct abuf *ab, const char* s, int len)
{
	char *new = realloc(ab->b, ab->len + len);

	if (new == NULL) return;
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void abFree(struct abuf* ab)
{
	free(ab->b);
}

void die(const char *s)
{
	perror(s);
	exit(1);
}
void enableRawMode(void)
{
	if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = E.orig_termios;
	
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 5;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1) die("tcsetattr");
}

void disableRawMode(void)
{
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");
}
char editorReadKey(void)
{
	int nread;
	char c;
	while((nread = read(STDIN_FILENO, &c, 1)) !=1){
		if(nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}

void editorProcessKeypress()
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

void editorDrawRows(struct abuf *ab)
{

	for(int y = 0; y < E.screenRows; y++){
		if(y == E.screenRows/3){
			char welcome[80];
			int welcomelen = snprintf(welcome, sizeof(welcome), "BMH editor version ", BMH_VERSION);
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



void initEditor(void)
{
	E.cx = 0;
	E.cy = 0;

	if(getWindowSize(&E.screenRows, &E.screenCols)==-1) die("getWindowSize");
}

int main(void)
{
	enableRawMode();
	initEditor();

	char c;
	while(1){
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}
