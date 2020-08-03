#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"
#include "utils/platform.h"
#include "config/arguments.h"
#include "config/config.h"
#include "process/process.h"

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

	FILE *configurationfile = checkfiles(arguments.configurationfile, defaultconfigurationfiles, defaultconfigurationcount);
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

	if (prepareparent(configuration))
	{
		return EXIT_FAILURE;
	}

	int programscount = countprogramlist(configuration->programs);
	Process *processes = (Process *)malloc(programscount * sizeof(Process));
	ProgramList *tip = configuration->programs;
	for (int i = 0; i < programscount; ++i, tip = tip->next)
	{
		processes[i].pid = 0;
		processes[i].config = tip->program;
	}

	// TODO: order by priority
	for (int i = 0; i < programscount; ++i)
	{
		if (processes[i].config.autostart)
		{
			openprocess(&processes[i]);
		}
	}

	readprocesses(processes, programscount);

	for (int i = 0; i < programscount; ++i)
	{
		closeprocess(&processes[i]);
	}

	freeconfiguration(configuration);
	return EXIT_SUCCESS;
}