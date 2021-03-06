#ifndef _ARGUMENTS_H_INCLUDED_
#define _ARGUMENTS_H_INCLUDED_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils/utils.h"

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
	int dryrun;
} ParsedArguments;

extern const Option options[];

ParsedArguments *parsearguments(int argc, char *argv[]);

void freearguments(ParsedArguments *arguments);

#endif
