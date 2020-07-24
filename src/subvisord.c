#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/arguments.h"
#include "config/config.h"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bugs@dgrn.tk>";
const struct argp argp = {options, parse_opt, 0, "subvisord -- run a set of applications as daemons.", 0, 0, 0};

#if defined(_WIN32) || defined(WIN32)
const char* defaultconfigurationfiles[] = {
	"../etc/subvisord.conf",
	"../etc/supervisord.conf",
	"../subvisord.conf",
	"../supervisord.conf",
	"./subvisord.conf",
	"./supervisord.conf",
	"./etc/subvisord.conf",
	"./etc/supervisord.conf",
};
#else
const char* defaultconfigurationfiles[] = {
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
#endif // _WIN32 || WIN32

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.configurationfile = NULL;
	memset(arguments.configurationlist, 0, MAX_ARGUMENTS * sizeof(char *));
	arguments.verbosity = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	FILE *configurationfile = checkfiles(arguments.configurationfile, defaultconfigurationfiles,
											   sizeof(defaultconfigurationfiles) / sizeof(defaultconfigurationfiles[0]));
	if (configurationfile == NULL)
	{
		// TODO: print message
		freearguments(arguments);
		fclose(configurationfile);
		return 1;
	}

	Configuration *configuration = readfromfile(configurationfile, arguments.configurationlist);
	freearguments(arguments);
	fclose(configurationfile);

	const char *error = validateconfiguration();

	if (!configuration)
	{
		// TODO: print message
		return 1;
	}

	freeconfiguration(configuration);

	return 0;
}