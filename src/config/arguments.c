#include "arguments.h"

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
	case 'v':
		arguments->verbosity = 1;
		break;
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