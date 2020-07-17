#include "arguments.h"

void addtoarguments(char ***arguments, char *arg)
{
	int i = 0;
	while ((*arguments)[i])
	{
		++i;
	}
	(*arguments)[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
	strcpy((*arguments)[i], arg);
}

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = (struct arguments *)state->input;
	char **configurations = arguments->configurationlist;

	char *buffer = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	memset(buffer, 0, MAX_LINE_LENGTH * sizeof(char));
	switch (key)
	{
	case 'v':
		arguments->verbosity = 1;
		break;
	case 'c':
		strcpy(arguments->configurationfile, arg);
		break;
	case 'n':
		addtoarguments(&configurations, "nodaemon=true");
		break;
	case 's':
		arguments->verbosity = -1;
		break;
	case 'u':
		sprintf(buffer, "user=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'm':
		sprintf(buffer, "umask=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'd':
		sprintf(buffer, "directory=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'l':
		sprintf(buffer, "logfile=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'y':
		sprintf(buffer, "logfile_maxbytes=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'z':
		sprintf(buffer, "logfile_backups=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'e':
		sprintf(buffer, "loglevel=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'j':
		sprintf(buffer, "pidfile=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'i':
		sprintf(buffer, "identifier=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'q':
		sprintf(buffer, "childlogdir=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 'k':
		addtoarguments(&configurations, "nocleanup=true");
		break;
	case 'a':
		sprintf(buffer, "minfds=%s", arg);
		addtoarguments(&configurations, buffer);
		break;
	case 't':
		sprintf(buffer, "strip_ansi=%s", arg);
		addtoarguments(&configurations, buffer);
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
	free(buffer);
	return 0;
}

void freearguments(struct arguments arguments) {
	free(arguments.configurationfile);
	for (int i = 0; arguments.configurationlist[i] != NULL; ++i) {
		free(arguments.configurationlist[i]);
	}
}