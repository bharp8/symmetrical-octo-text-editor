#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <termios.h>

struct termios orig_termios;

void disableRawMode(void);

void die(const char *s)
{
	perror(s);
	exit(1);
}
void enableRawMode(void)
{
	if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	
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
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
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
			exit(0);
			break;
	}
}

int main(void)
{
	enableRawMode();

	char c;
	while(1){
		editorProcessKeypress();
	}
	return 0;
}
