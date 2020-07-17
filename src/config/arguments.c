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
	char **arguments = (char **)state->input;

	char *buffer = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	memset(buffer, 0, MAX_LINE_LENGTH * sizeof(char));
	switch (key)
	{
	case 'v':
		addtoarguments(&arguments, "verbose=true");
		break;
	case 'c':
		sprintf(buffer, "configuration=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'n':
		addtoarguments(&arguments, "nodaemon=true");
		break;
	case 's':
		addtoarguments(&arguments, "silent=true");
		break;
	case 'u':
		sprintf(buffer, "user=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'm':
		sprintf(buffer, "umask=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'd':
		sprintf(buffer, "directory=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'l':
		sprintf(buffer, "logfile=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'y':
		sprintf(buffer, "logfile_maxbytes=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'z':
		sprintf(buffer, "logfile_backups=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'e':
		sprintf(buffer, "loglevel=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'j':
		sprintf(buffer, "pidfile=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'i':
		sprintf(buffer, "identifier=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'q':
		sprintf(buffer, "childlogdir=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 'k':
		addtoarguments(&arguments, "nocleanup=true");
		break;
	case 'a':
		sprintf(buffer, "minfds=%s", arg);
		addtoarguments(&arguments, buffer);
		break;
	case 't':
		sprintf(buffer, "strip_ansi=%s", arg);
		addtoarguments(&arguments, buffer);
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
