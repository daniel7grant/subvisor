#include <stdio.h>
#include <stdlib.h>

#include "config/arguments.h"
#include "config/config.h"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bugs@dgrn.tk>";
const struct argp argp = {options, parse_opt, 0, "subvisord -- run a set of applications as daemons.", 0, 0, 0};

int main(int argc, char **argv)
{
	char *arguments[MAX_ARGUMENTS];
	memset(arguments, 0, MAX_ARGUMENTS*sizeof(char *));
	arguments[0] = "nodaemon=true";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	Configuration *configuration = readfromfile("supervisord.conf", arguments);
	
	return 0;
}