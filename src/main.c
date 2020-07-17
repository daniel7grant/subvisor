#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/arguments.h"
#include "config/config.h"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bugs@dgrn.tk>";
const struct argp argp = {options, parse_opt, 0, "subvisord -- run a set of applications as daemons.", 0, 0, 0};

const char *defaultconfigurationfiles[] = {
	"../etc/subvisord.conf",
	"../etc/supervisord.conf",
	"../subvisord.conf",
	"../supervisord.conf",
	"./subvisord.conf",
	"./supervisord.conf",
	"./etc/subvisord.conf",
	"./etc/supervisord.conf",
	"/etc/subvisord.conf",
	"/etc/supervisord.conf",
	"/etc/subvisor/subvisord.conf",
	"/etc/supervisor/supervisord.conf",
};

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.configurationfile = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	memset(arguments.configurationfile, 0, MAX_LINE_LENGTH * sizeof(char));
	memset(arguments.configurationlist, 0, MAX_ARGUMENTS * sizeof(char *));
	arguments.verbosity = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	char *configurationfile = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	if (strlen(arguments.configurationfile) > 0)
	{
		if (access(arguments.configurationfile, R_OK))
		{
			// TODO: print to the console
			return 1;
		}
		strcpy(configurationfile, arguments.configurationfile);
	}
	else
	{
		int i = 0, length = sizeof(defaultconfigurationfiles) / sizeof(defaultconfigurationfiles[i]);
		while (access(defaultconfigurationfiles[i], R_OK) && i < length)
		{
			++i;
		}
		if (i >= length)
		{
			return 1;
		}
		strcpy(configurationfile, defaultconfigurationfiles[i]);
	}
	Configuration *configuration = readfromfile(configurationfile, arguments.configurationlist);
	if (!configuration)
	{
		return 1;
	}

	freearguments(arguments);

	return 0;
}