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
	handlesignals();
	handleprocesssignals();

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

	processcount = countprogramlist(configuration->programs);
	processes = (Process *)malloc(processcount * sizeof(Process));
	printf("%ld", sizeof(Process));
	ProgramList *tip = configuration->programs;
	for (int i = 0; i < processcount; ++i, tip = tip->next)
	{
		processes[i].pid = 0;
		processes[i].config = &tip->program;
		processes[i].state = STOPPED;
		processes[i].retries = 0;
	}

	// TODO: order by priority
	// TODO: start multiple processes
	for (int i = 0; i < processcount; ++i)
	{
		if (processes[i].config->autostart)
		{
			openprocess(&processes[i]);
		}
	}

	// TODO: time may be too imprecise
	time_t now;
	while (readprocesses(processes, processcount) == 0)
	{
		time(&now);
		for (int i = 0; i < processcount; ++i)
		{
			if (processes[i].state == STARTING && difftime(now, processes[i].starttime) > processes[i].config->startsecs)
			{
				processes[i].state = RUNNING;
			}
			else if (processes[i].state == STOPPING)
			{
				processes[i].state = STOPPED;
			}
			else if (processes[i].state == BACKOFF)
			{
				openprocess(&processes[i]);
			}
		}

		for (int i = 0; i < processcount; ++i)
		{
			// fprintf(stderr, "%s (%d): %d\n", processes[i].config->process_name, processes[i].pid, processes[i].state);
		}
	}

	for (int i = 0; i < processcount; ++i)
	{
		closeprocess(&processes[i]);
	}

	free(processes);
	freeconfiguration(configuration);
	return EXIT_SUCCESS;
}