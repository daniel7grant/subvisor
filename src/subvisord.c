#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "config/arguments.h"
#include "config/config.h"

#if defined(_WIN32) || defined(WIN32)
const char *defaultconfigurationfiles[] = {
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
#endif // _WIN32 || WIN32

int main(int argc, char **argv)
{
	arg0 = argv[0];

	ParsedArguments arguments;
	arguments.configurationfile = NULL;
	memset(arguments.configurationlist, 0, MAX_ARGUMENTS * sizeof(char *));
	arguments.verbosity = 1;
	if (parsearguments(&arguments, argc, argv))
	{
		return EXIT_FAILURE;
	}

	FILE *configurationfile = checkfiles(arguments.configurationfile, defaultconfigurationfiles,
										 sizeof(defaultconfigurationfiles) / sizeof(defaultconfigurationfiles[0]));
	if (configurationfile == NULL)
	{
		if (arguments.configurationfile != NULL)
		{
			usage("could not find config file %s.", arguments.configurationfile);
		}
		else
		{
			usage("could not find config file.");
		}
		freearguments(arguments);
		return EXIT_FAILURE;
	}

	Configuration *configuration = readfromfile(configurationfile, arguments.configurationlist);
	freearguments(arguments);
	fclose(configurationfile);

	if (!configuration)
	{
		return EXIT_FAILURE;
	}

	if (validateconfiguration(configuration))
	{
		freeconfiguration(configuration);
		return EXIT_FAILURE;
	}

	freeconfiguration(configuration);

	return EXIT_SUCCESS;
}