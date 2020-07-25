#ifndef _ARGUMENTS_H_INCLUDED_
#define _ARGUMENTS_H_INCLUDED_

#include <argp.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGUMENTS 100
#define MAX_LINE_LENGTH 1024

extern const struct argp_option options[];

struct arguments {
	char *configurationlist[MAX_ARGUMENTS];
	char *configurationfile;
	int verbosity;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

void freearguments(struct arguments arguments);

#endif
