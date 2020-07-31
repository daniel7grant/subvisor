#ifndef _ARGUMENTS_H_INCLUDED_
#define _ARGUMENTS_H_INCLUDED_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils/utils.h"

#define SUBVISORD_VERSION "0.0.0"
#define MAX_ARGUMENTS 100
#define MAX_LINE_LENGTH 1024

typedef struct Option {
	const char *name;
	char key;
	const char *args;
	const char *docs;
} Option;

typedef struct ParsedArguments {
	char *configurationlist[MAX_ARGUMENTS];
	char *configurationfile;
	int verbosity;
} ParsedArguments;

extern const Option options[];

int parsearguments(ParsedArguments* arguments, int argc, char *argv[]);

void freearguments(ParsedArguments arguments);

#endif
