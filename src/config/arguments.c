#include "arguments.h"

const Option options[] = {
	{"configuration", 'c', "FILENAME", "configuration file path (searches if not given)"},
	{"nodaemon", 'n', 0, "run in the foreground (same as 'nodaemon=true' in config file)"},
	{"silent", 's', 0, "no logs to stdout (maps to 'silent=true' in config file)"},
	{"minfds", 'a', "NUM", "the minimum number of file descriptors for start success"},
	{"directory", 'd', "DIR", "directory to chdir to when daemonized"},
	{"loglevel", 'e', "LEVEL", "use LEVEL as log level (debug,info,warn,error,critical)"},
	{"identifier", 'i', "STR", "identifier used for this instance of supervisord"},
	{"pidfile", 'j', "FILENAME", "write a pid file for the daemon process to FILENAME"},
	{"nocleanup", 'k', 0, "prevent the process from performing cleanup (removal of old automatic child log files) at startup."},
	{"logfile", 'l', "FILENAME", "use FILENAME as logfile path"},
	{"logfile_maxbytes", 'y', "BYTES", "use BYTES to limit the max size of logfile"},
	{"logfile_backups", 'z', "NUM", "number of backups to keep when max bytes reached"},
	{"umask", 'm', "UMASK", "use this umask for daemon subprocess (default is 022)"},
	{"childlogdir", 'q', "DIR", "the log directory for child process logs"},
	{"strip_ansi", 't', 0, "strip ansi escape codes from process output"},
	{"user", 'u', "USER", "run supervisord as this user (or numeric uid)"},
	{"minprocs", 0, "NUM", "the minimum number of processes available for start success"},
	{"help", 'h', 0, "show this help"},
	{"version", 'V', 0, "print program version"},
	{"dry-run", 'T', 0, "test configuration file (dry-run)"},
};

Option nullopt = {0};

Option findoptshort(char key)
{
	int optlength = sizeof(options) / sizeof(options[0]);
	for (int i = 0; i < optlength; ++i)
	{
		if (options[i].key == key)
		{
			return options[i];
		}
	}
	return nullopt;
}

Option findoptlong(char *name)
{
	int optlength = sizeof(options) / sizeof(options[0]);
	for (int i = 0; i < optlength; ++i)
	{
		if (options[i].name && strcmp(options[i].name, name) == 0)
		{
			return options[i];
		}
	}
	return nullopt;
}

void handleoption(ParsedArguments *arguments, Option opt, char *arg)
{
	switch (opt.key)
	{
	case 'h':
	{
		int optlength = sizeof(options) / sizeof(options[0]);
		printf(
			"Usage: %s [OPTION...]\n"
			"Run a set of applications as daemons.\n"
			"\n"
			"Options:\n",
			arg0);

		char *optionstring = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
		memset(optionstring, 0, MAX_LINE_LENGTH);
		for (int i = 0; i < optlength; ++i)
		{
			sprintf(
				optionstring,
				"  %s%c%s %s%s%s%s",
				options[i].key ? "-" : "  ",
				options[i].key ? options[i].key : ' ',
				options[i].key && options[i].name ? "," : "",
				options[i].name ? "--" : "",
				options[i].name ? options[i].name : "",
				options[i].name && options[i].args ? "=" : "",
				options[i].args ? options[i].args : "");
			printf("%-30s  %s\n", optionstring, options[i].docs);
		}
		free(optionstring);

		puts("\nFor more information check out the documentation at https://github.com/daniel7grant/subvisor.");
		exit(0);
	}
	case 'V':
	{
		printf("%s\n", SUBVISORD_VERSION);
		exit(0);
	}
	case 'T':
	{
		arguments->dryrun = 1;
		break;
	}
	case 'c':
	{
		arguments->configurationfile = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
		strcpy(arguments->configurationfile, arg);
		break;
	}
	case 's':
	{
		arguments->verbosity = 0;
		break;
	}
	default:
	{
		int i = 0;
		while (arguments->configurationlist[i])
		{
			++i;
		}
		arguments->configurationlist[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
		sprintf(arguments->configurationlist[i], "%s=%s", opt.name, opt.args != NULL ? arg : "true");
		break;
	}
	}
}

ParsedArguments *parsearguments(int argc, char *argv[])
{
	ParsedArguments *arguments = (ParsedArguments *)malloc(sizeof(ParsedArguments));
	arguments->configurationfile = NULL;
	memset(arguments->configurationlist, 0, MAX_ARGUMENTS * sizeof(char *));
	arguments->verbosity = 1;
	arguments->dryrun = 0;

	char *current, *next;
	for (int i = 1; i < argc; ++i)
	{
		current = argv[i];
		next = i < argc ? argv[i + 1] : NULL;
		if (current[0] == '-')
		{
			if (current[1] == '-')
			{
				// long options
				if (strlen(current) == 2)
				{
					// end of arguments
					return arguments;
				}
				next = strchr(current, '=');
				if (next != NULL)
				{
					// argument inside option
					// set next to the arg
					(*next) = '\0';
					++next;
				}
				Option opt = findoptlong(&(current[2]));
				if (opt.name == NULL)
				{
					usage("option %s not recognized", current);
					return NULL;
				}
				if (opt.args != NULL)
				{
					// argument after option
					if (next == NULL)
					{
						usage("option %s requires argument", current);
						return NULL;
					}
					handleoption(arguments, opt, next);
					// skip argument
					++i;
				}
				else
				{
					handleoption(arguments, opt, NULL);
				}
			}
			else
			{
				// short options
				int j = 1;
				while (current[j] != '\0')
				{
					Option opt = findoptshort(current[j]);
					if (opt.key != current[j])
					{
						usage("option %s not recognized", current);
						return NULL;
					}
					if (opt.args == NULL)
					{
						// no arguments
						handleoption(arguments, opt, NULL);
					}
					else if (current[j + 1] != '\0')
					{
						// argument inside option
						handleoption(arguments, opt, &(current[j + 1]));
						break;
					}
					else
					{
						// argument after option
						if (next == NULL)
						{
							usage("option %s requires argument", current);
							return NULL;
						}
						// skip argument
						++i;
						handleoption(arguments, opt, next);
					}
					++j;
				}
			}
		}
		else
		{
			usage("positional arguments are not supported: %s", current);
			// We can use that our program should not have arguments
			return NULL;
		}
	}
	return arguments;
}

void freearguments(ParsedArguments *arguments)
{
	if (arguments->configurationfile != NULL)
	{
		free(arguments->configurationfile);
	}
	for (int i = 0; arguments->configurationlist[i] != NULL; ++i)
	{
		free(arguments->configurationlist[i]);
	}
}