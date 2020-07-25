#include "arguments.h"

const struct argp_option options[] = {
	{"configuration", 'c', "FILENAME", 0, "configuration file path (searches if not given)", 0},
	{"nodaemon", 'n', 0, 0, "run in the foreground (same as 'nodaemon=true' in config file)", 0},
	{"silent", 's', 0, 0, "no logs to stdout (maps to 'silent=true' in config file)", 0},
	{"user", 'u', "USER", 0, "run supervisord as this user (or numeric uid)", 0},
	{"umask", 'm', "UMASK", 0, "use this umask for daemon subprocess (default is 022)", 0},
	{"directory", 'd', "DIR", 0, "directory to chdir to when daemonized", 0},
	{"logfile", 'l', "FILENAME", 0, "use FILENAME as logfile path", 0},
	{"logfile_maxbytes", 'y', "BYTES", 0, "use BYTES to limit the max size of logfile", 0},
	{"logfile_backups", 'z', "NUM", 0, "number of backups to keep when max bytes reached", 0},
	{"loglevel", 'e', "LEVEL", 0, "use LEVEL as log level (debug,info,warn,error,critical)", 0},
	{"pidfile", 'j', "FILENAME", 0, "write a pid file for the daemon process to FILENAME", 0},
	{"identifier", 'i', "STR", 0, "identifier used for this instance of supervisord", 0},
	{"childlogdir", 'q', "DIR", 0, "the log directory for child process logs", 0},
	{"nocleanup", 'k', 0, 0, "prevent the process from performing cleanup (removal of old automatic child log files) at startup.", 0},
	{"minfds", 'a', "NUM", 0, "the minimum number of file descriptors for start success", 0},
	{"strip_ansi", 't', 0, 0, "strip ansi escape codes from process output", 0},
	{"minprocs", 0, "NUM", 0, "the minimum number of processes available for start success", 0},
	{"profile_options", 0, "OPTS", 0, "run supervisord under profiler and output results based on OPTIONS, which  is a comma-sep'd list of 'cumulative', 'calls', and/or 'callers', e.g. 'cumulative,callers')", 0},
	{0}};

void addtoarguments(char ***arguments, const char *key, char *arg)
{
	int i = 0;
	while ((*arguments)[i])
	{
		++i;
	}
	(*arguments)[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
	sprintf((*arguments)[i], key, arg);
}

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = (struct arguments *)state->input;
	char **configurations = arguments->configurationlist;

	switch (key)
	{
	case 'c':
		arguments->configurationfile = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
		strcpy(arguments->configurationfile, arg);
		break;
	case 'n':
		addtoarguments(&configurations, "nodaemon=%s", "true");
		break;
	case 's':
		arguments->verbosity = -1;
		break;
	case 'u':
		addtoarguments(&configurations, "user=%s", arg);
		break;
	case 'm':
		addtoarguments(&configurations, "umask=%s", arg);
		break;
	case 'd':
		addtoarguments(&configurations, "directory=%s", arg);
		break;
	case 'l':
		addtoarguments(&configurations, "logfile=%s", arg);
		break;
	case 'y':
		addtoarguments(&configurations, "logfile_maxbytes=%s", arg);
		break;
	case 'z':
		addtoarguments(&configurations, "logfile_backups=%s", arg);
		break;
	case 'e':
		addtoarguments(&configurations, "loglevel=%s", arg);
		break;
	case 'j':
		addtoarguments(&configurations, "pidfile=%s", arg);
		break;
	case 'i':
		addtoarguments(&configurations, "identifier=%s", arg);
		break;
	case 'q':
		addtoarguments(&configurations, "childlogdir=%s", arg);
		break;
	case 'k':
		addtoarguments(&configurations, "nocleanup=%s", "true");
		break;
	case 'a':
		addtoarguments(&configurations, "minfds=%s", arg);
		break;
	case 't':
		addtoarguments(&configurations, "strip_ansi=%s", arg);
		break;
		// minprocs
		// profile_options

	case ARGP_KEY_ARG:
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

void freearguments(struct arguments arguments)
{
	if (arguments.configurationfile != NULL)
	{
		free(arguments.configurationfile);
	}
	for (int i = 0; arguments.configurationlist[i] != NULL; ++i)
	{
		free(arguments.configurationlist[i]);
	}
}