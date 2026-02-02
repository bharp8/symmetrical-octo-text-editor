#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "errors.h"
#include "fileIO.h"
#include "editorConfig.h"
#include "rows.h"
#include "rawMode.h"
#include "draw.h"

void die(const char *s)
{
	perror(s);
	exit(1);
}

