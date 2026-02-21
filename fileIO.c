#include <stdio.h>
#include "errors.h"
#include <stdlib.h>
#include "rows.h"
#include <stdlib.h>
#include "editorConfig.h"
#include <string.h>

void editorOpen(char * filename)
{
	free(E.filename);
	E.filename = strdup(filename);

	FILE *fp = fopen(filename, "r");
	if(!fp) die("fopen");

	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while((linelen = getline(&line, &linecap, fp))!=-1){
		while(linelen > 0 && (line[linelen-1] == '\n' || line[linelen-1] == '\r')) linelen --;
		editorAppendRow(line, linelen);
	}
	free(line);
	fclose(fp);
}


