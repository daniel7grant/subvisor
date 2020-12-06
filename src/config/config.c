#include "config.h"
#include "../utils/utils.h"
#include "../utils/platform.h"

int endofline(char c)
{
	return c == '\0' || c == '\r' || c == '\n' || c == '#' || c == ';';
}

int whitespace(char c)
{
	return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

PARSE_RETURN initializeblock(Configuration *configuration, char *block, int included)
{
	if (strcmp(block, "subvisord") == 0 || strcmp(block, "supervisord") == 0)
	{
		if (!configuration->subvisord)
		{
			char *cwdpid = getcurrentdirfile("supervisord.pid");

			configuration->subvisord = 1;
			strcpy(configuration->pidfile, cwdpid);
			configuration->nodaemon = 0;
			configuration->minfds = 1024;
			configuration->minprocs = 200;
			configuration->umask = 0220;
			strcpy(configuration->user, "");
			strcpy(configuration->directory, "");
			configuration->strip_ansi = 0;
			strcpy(configuration->identifier, "subvisor");
			strcpy(configuration->environment, "");
			configuration->nocleanup = 0;
			configuration->programs = NULL;

			const char *tempdir = gettempdir();
			strcpy(configuration->childlogdir, tempdir);

			char *cwdlog = getcurrentdirfile("supervisord.log");
			Logger log = createlogger(cwdlog);
			log.loglevel = 2;
			configuration->log = log;

			free(cwdpid);
			free(cwdlog);
		}
	}
	else if (strcmp(block, "include") == 0)
	{
		if (included)
		{
			return PARSE_ERROR_INCLUDE;
		}
	}
	else if (strncmp(block, "program:", strlen("program:")) == 0)
	{
		configuration->programs = pushtoprogramlist(configuration->programs, createdefaultprogramconfig(block + strlen("program:")));
	}
	else if (strncmp(block, "fcgi-program", strlen("fcgi-program")) == 0 ||
			 strncmp(block, "eventlistener", strlen("eventlistener")) == 0 ||
			 strncmp(block, "rpcinterface", strlen("rpcinterface")) == 0)
	{
		// MIGHT HAPPEN
	}
	else if (strncmp(block, "group", strlen("group")) == 0 ||
			 strcmp(block, "unix_http_server") == 0 ||
			 strcmp(block, "inet_http_server") == 0 ||
			 strcmp(block, "supervisorctl") == 0)
	{
		// TODO: WARNING: WILL NOT WORK
		// HOWEVER FAIL SILENTLY TO MAINTAIN COMPATIBILITY
	}
	else
	{
		return PARSE_ERROR_BAD_BLOCK;
	}
	return PARSE_SUCCESS;
}

PARSE_RETURN setconfigvariable(Configuration *configuration, char *block, char *key, char *value)
{
	if (strcmp(block, "subvisord") == 0 || strcmp(block, "supervisord") == 0)
	{
		if (strcmp(key, "pidfile") == 0)
		{
			strcpy(configuration->pidfile, value);
		}
		else if (strcmp(key, "nodaemon") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			configuration->nodaemon = boolval;
		}
		else if (strcmp(key, "minfds") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			configuration->minfds = numval;
		}
		else if (strcmp(key, "minprocs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			configuration->minprocs = numval;
		}
		else if (strcmp(key, "umask") == 0)
		{
			int numval = tonumber(value, 8);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			configuration->umask = numval;
		}
		else if (strcmp(key, "user") == 0)
		{
			strcpy(configuration->user, value);
		}
		else if (strcmp(key, "directory") == 0)
		{
			strcpy(configuration->directory, value);
		}
		else if (strcmp(key, "strip_ansi") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			configuration->strip_ansi = boolval;
		}
		else if (strcmp(key, "identifier") == 0)
		{
			strcpy(configuration->identifier, value);
		}
		else if (strcmp(key, "environment") == 0)
		{
			// TODO: tokenize environment string
			strcpy(configuration->environment, value);
		}
		else if (strcmp(key, "nocleanup") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			configuration->nocleanup = boolval;
		}
		else if (strcmp(key, "childlogdir") == 0)
		{
			strcpy(configuration->childlogdir, value);
		}
		else if (strcmp(key, "logfile") == 0)
		{
			strcpy(configuration->log.logfile, value);
		}
		else if (strcmp(key, "logfile_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return PARSE_ERROR_BYTE;
			}
			configuration->log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "loglevel") == 0)
		{
			int logval = tologlevel(value);
			if (logval < 0)
			{
				return PARSE_ERROR_LOGLEVEL;
			}
			configuration->log.loglevel = logval;
		}
		else if (strcmp(key, "logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			configuration->log.logfile_backups = numval;
		}
		else
		{
			return PARSE_ERROR_BAD_KEY;
		}
	}
	else if (strcmp(block, "include") == 0)
	{
		if (strcmp(key, "files") == 0)
		{
			configuration->included_files = toglob(value);
		}
		else
		{
			return PARSE_ERROR_BAD_KEY;
		}
	}
	else if (strncmp(block, "program:", strlen("program:")) == 0)
	{
		ProgramList *programlist = peekprogramlist(configuration->programs);
		if (strcmp(key, "command") == 0)
		{
			strcpy(programlist->program.command, value);
		}
		else if (strcmp(key, "process_name") == 0)
		{
			strcpy(programlist->program.process_name, value);
		}
		else if (strcmp(key, "numprocs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.numprocs = numval;
		}
		else if (strcmp(key, "numprocs_start") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.numprocs_start = numval;
		}
		else if (strcmp(key, "umask") == 0)
		{
			int numval = tonumber(value, 8);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.umask = numval;
		}
		else if (strcmp(key, "priority") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.priority = numval;
		}
		else if (strcmp(key, "autostart") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.autostart = boolval;
		}
		else if (strcmp(key, "autorestart") == 0)
		{
			int restartval = toautorestart(value);
			if (restartval < 0)
			{
				return PARSE_ERROR_AUTORESTART;
			}
			programlist->program.autorestart = restartval;
		}
		else if (strcmp(key, "startsecs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.startsecs = numval;
		}
		else if (strcmp(key, "startretries") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.startretries = numval;
		}
		else if (strcmp(key, "exitcodes") == 0)
		{
			short *exitcodes = toexitcode(value);
			if (exitcodes == NULL)
			{
				return PARSE_ERROR_EXITCODE;
			}
			memcpy(programlist->program.exitcodes, exitcodes, MAX_EXITCODES * sizeof(short));
		}
		else if (strcmp(key, "stopsignal") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.stopsignal = numval;
		}
		else if (strcmp(key, "stopwaitsecs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.stopwaitsecs = numval;
		}
		else if (strcmp(key, "stopasgroup") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.stopasgroup = boolval;
		}
		else if (strcmp(key, "killasgroup") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.killasgroup = boolval;
		}
		else if (strcmp(key, "redirect_stderr") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.redirect_stderr = boolval;
		}
		else if (strcmp(key, "user") == 0)
		{
			strcpy(programlist->program.user, value);
		}
		else if (strcmp(key, "directory") == 0)
		{
			strcpy(programlist->program.directory, value);
		}
		else if (strcmp(key, "environment") == 0)
		{
			strcpy(programlist->program.environment, value);
		}
		else if (strcmp(key, "stdout_logfile") == 0)
		{
			strcpy(programlist->program.stdout_log.logfile, value);
		}
		else if (strcmp(key, "stdout_logfile_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return PARSE_ERROR_BYTE;
			}
			programlist->program.stdout_log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "stdout_logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.stdout_log.logfile_backups = numval;
		}
		else if (strcmp(key, "stdout_capture_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return PARSE_ERROR_BYTE;
			}
			programlist->program.stdout_log.capture_maxbytes = byteval;
		}
		else if (strcmp(key, "stdout_syslog") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.stdout_log.syslog = boolval;
		}
		else if (strcmp(key, "stdout_events_enabled") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.stdout_log.events_enabled = boolval;
		}
		else if (strcmp(key, "stderr_logfile") == 0)
		{
			strcpy(programlist->program.stderr_log.logfile, value);
		}
		else if (strcmp(key, "stderr_logfile_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return PARSE_ERROR_BYTE;
			}
			programlist->program.stderr_log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "stderr_logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return PARSE_ERROR_NUMBER;
			}
			programlist->program.stderr_log.logfile_backups = numval;
		}
		else if (strcmp(key, "stderr_capture_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return PARSE_ERROR_BYTE;
			}
			programlist->program.stderr_log.capture_maxbytes = byteval;
		}
		else if (strcmp(key, "stderr_syslog") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.stderr_log.syslog = boolval;
		}
		else if (strcmp(key, "stderr_events_enabled") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return PARSE_ERROR_BOOLEAN;
			}
			programlist->program.stderr_log.events_enabled = boolval;
		}
		else if (strcmp(key, "serverurl") == 0)
		{
			// TODO: WARNING: WILL NOT WORK
			// HOWEVER FAIL SILENTLY TO MAINTAIN COMPATIBILITY
		}
		else
		{
			return PARSE_ERROR_BAD_KEY;
		}
	}
	else if (strncmp(block, "fcgi-program", strlen("fcgi-program")) == 0 ||
			 strncmp(block, "eventlistener", strlen("eventlistener")) == 0 ||
			 strncmp(block, "rpcinterface", strlen("rpcinterface")) == 0)
	{
		// MIGHT HAPPEN
		// HOWEVER FAIL SILENTLY TO MAINTAIN COMPATIBILITY
	}
	else if (strncmp(block, "group", strlen("group")) == 0 ||
			 strcmp(block, "unix_http_server") == 0 ||
			 strcmp(block, "inet_http_server") == 0 ||
			 strcmp(block, "supervisorctl") == 0)
	{
		// WILL NOT WORK
		// HOWEVER FAIL SILENTLY TO MAINTAIN COMPATIBILITY
	}
	else
	{
		return PARSE_ERROR_BAD_BLOCK;
	}
	return PARSE_SUCCESS;
}

PARSE_RETURN parseline(char *line, char *pair[2])
{
	int i = 0, key = 0, value = 0, end = 0;

	// SKIP STARTING WHITESPACE
	while (whitespace(line[i]))
	{
		key = ++i;
	}

	// IF STARTS WITH COMMENT SIGN, SKIP
	if (endofline(line[i]))
	{
		pair[0] = NULL;
		pair[1] = NULL;
		return PARSE_SUCCESS;
	}

	// IF STARTS WITH '[', CREATE NEW BLOCK
	if (line[i] == '[')
	{
		key = ++i;
		while (line[i] != ']' && !endofline(line[i]))
		{
			++i;
		}
		if (endofline(line[i]))
		{
			pair[0] = NULL;
			pair[1] = NULL;
			return PARSE_ERROR_BAD_BLOCK;
		}
		line[i] = '\0';

		pair[0] = &line[key];
		pair[1] = NULL;
		return PARSE_SUCCESS_NEW_BLOCK;
	}

	// GO UNTIL THERE IS AN EQUAL SIGN OR EOL
	while (line[i] != '=' && !endofline(line[i]))
	{
		if (!whitespace(line[i]))
		{
			end = i;
		}
		++i;
	}

	// FAIL IF THERE IS NO KEY-VALUE PAIR
	if (endofline(line[i]))
	{
		pair[0] = NULL;
		pair[1] = NULL;
		return PARSE_ERROR;
	}
	line[end + 1] = '\0';

	do
	{
		value = ++i;
	} while (whitespace(line[i]));

	while (!endofline(line[i]))
	{
		if (!whitespace(line[i]))
		{
			end = i;
		}
		++i;
	}
	line[end + 1] = '\0';

	pair[0] = &line[key];
	pair[1] = &line[value];
	return PARSE_SUCCESS_NEW_KEY;
}

FILE *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length)
{
	FILE *file = NULL;
	if (configurationfile != NULL)
	{
		file = fopen(configurationfile, "r");
	}
	else
	{
		int i = 0;
		while (i < length && (file = fopen(defaultconfigurationfiles[i], "r")) == NULL)
		{
			++i;
		}
	}
	return file;
}

int parsefromfile(Configuration *configuration, FILE *conffile, char *conffilename, int included)
{
	PARSE_RETURN parsing_result = 0;
	char *block = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	int linenumber = 1;
	char *pair[2];
	while (fgets(line, MAX_LINE_LENGTH, conffile) != NULL)
	{
		parsing_result = parseline(line, pair);

		if (parsing_result == PARSE_SUCCESS_NEW_BLOCK)
		{
			strcpy(block, pair[0]);
			parsing_result = initializeblock(configuration, block, included);
		}
		else if (parsing_result == PARSE_SUCCESS_NEW_KEY)
		{
			parsing_result = setconfigvariable(configuration, block, pair[0], pair[1]);
		}

		if (parsing_result)
		{
			// PARSING FAILED
			char *debugline = line;
			if (pair[1] != NULL)
			{
				sprintf(debugline, "%s = %s", pair[0], pair[1]);
			}
			switch (parsing_result)
			{
			case PARSE_ERROR_INCLUDE:
				usage("included file cannot include again: '%s'\n\t[line %d]: %s", conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_BOOLEAN:
				usage("invalid boolean value: '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_NUMBER:
				usage("invalid numeric value: '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_BYTE:
				usage("invalid size value (allowed units: numbers, B, kB, MB, GB): '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_LOGLEVEL:
				usage("bad logging level name: '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_EXITCODE:
				usage("not a valid list of exit codes: '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_AUTORESTART:
				usage("invalid autorestart value: '%s' (file: '%s')\n\t[line %d]: %s", pair[1], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_BAD_BLOCK:
				usage("invalid block: '%s' (file: '%s')\n\t[line %d]: %s", pair[0], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR_BAD_KEY:
				usage("invalid key in block %s: '%s' (file: '%s')\n\t[line %d]: %s", block, pair[0], conffilename, linenumber, debugline);
				break;
			case PARSE_ERROR:
			default:
				usage("configuration file contains parsing errors: '%s'\n\t[line %d]: %s", conffilename, linenumber, debugline);
				break;
			}
			free(block);
			free(line);
			return EXIT_FAILURE;
		}

		++linenumber;
	}

	free(block);
	free(line);

	return EXIT_SUCCESS;
}

int parsefromargs(Configuration *configuration, char **arguments)
{
	PARSE_RETURN parsing_result = 0;
	char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));

	// MERGE WITH ARGUMENTS FROM THE COMMAND LINE
	int k = 0;
	while (arguments[k] != NULL)
	{
		char *pair[2];
		strcpy(line, arguments[k]);
		parsing_result = parseline(line, pair);
		if (parsing_result == PARSE_SUCCESS_NEW_KEY)
		{
			// ADD NEW KEY-VALUE PAIR
			parsing_result = setconfigvariable(configuration, "subvisord", pair[0], pair[1]);
		}

		if (parsing_result)
		{
			// CONFIGURATION FAILED
			switch (parsing_result)
			{
			case PARSE_ERROR_BOOLEAN:
				usage("invalid boolean value: '%s'", pair[1]);
				break;
			case PARSE_ERROR_NUMBER:
				usage("invalid numeric value: '%s'", pair[1]);
				break;
			case PARSE_ERROR_BYTE:
				usage("invalid size value (allowed units: numbers, B, kB, MB, GB): '%s'", pair[1]);
				break;
			case PARSE_ERROR_LOGLEVEL:
				usage("bad logging level name: '%s'", pair[1]);
				break;
			case PARSE_ERROR_EXITCODE:
				usage("not a valid list of exit codes: '%s'", pair[1]);
				break;
			case PARSE_ERROR_AUTORESTART:
				usage("invalid autorestart value: '%s'", pair[1]);
				break;
			case PARSE_ERROR:
			default:
				usage("command line arguments contains parsing errors: \n\t%s=%s", pair[0], pair[1]);
				break;
			}
			free(line);
			return EXIT_FAILURE;
		}
		++k;
	}

	free(line);
	return EXIT_SUCCESS;
}

int validateconfiguration(Configuration *configuration)
{
	if (!configuration->subvisord)
	{
		usage("%s", ".ini file does not include subvisord section");
		return EXIT_FAILURE;
	}

	if (openlogger(&configuration->log))
	{
		usage("could not write log file %s", configuration->log.logfile);
		return EXIT_FAILURE;
	}

	if (strlen(configuration->childlogdir) > 0 && !checkaccess(configuration->childlogdir, 0))
	{
		usage("%s is not an existing directory", configuration->childlogdir);
		return EXIT_FAILURE;
	}

	if (strlen(configuration->directory) > 0 && !checkaccess(configuration->directory, 0))
	{
		usage("%s is not an existing directory", configuration->directory);
		return EXIT_FAILURE;
	}

	int uid = getcurrentuserid();
	if (strlen(configuration->user) > 0)
	{
		int supervisoruid = getuserid(configuration->user);

		if (uid > 0 && uid != supervisoruid)
		{
			usage("%s", "can't drop privilege as nonroot user");
			return EXIT_FAILURE;
		}

		if (supervisoruid < 0)
		{
			usage("invalid user name %s", configuration->user);
			return EXIT_FAILURE;
		}
	}
	else if (uid == 0)
	{
		usage("%s", "subvisor is running as root. Privileges were not dropped because no user is specified in the config file. If you intend to run as root, you can set user=root in the config file to avoid this message.");
		return EXIT_FAILURE;
	}

	ProgramList *programlist = configuration->programs;
	while (programlist != NULL)
	{
		if (strlen(programlist->program.command) == 0)
		{
			usage("program section %s does not specify a command", programlist->program.process_name);
			return EXIT_FAILURE;
		}

		if (strlen(programlist->program.directory) > 0 && !checkaccess(programlist->program.directory, 0))
		{
			usage("%s is not an existing directory", programlist->program.directory);
			return EXIT_FAILURE;
		}

		if (strlen(programlist->program.user) > 0 && getuserid(programlist->program.user) < 0)
		{
			usage("invalid user name %s", programlist->program.user);
			return EXIT_FAILURE;
		}

		if (programlist->program.numprocs > 1 && strstr(programlist->program.process_name, "%(process_num)"))
		{
			usage("%s", "%(process_num) must be present within process_name when numprocs > 1");
			return EXIT_FAILURE;
		}

		if (programlist->program.stopasgroup && !programlist->program.killasgroup)
		{
			usage("%s", "cannot set stopasgroup=true and killasgroup=false");
			return EXIT_FAILURE;
		}

		if (openlogger(&programlist->program.stdout_log))
		{
			usage("could not write stdout log file %s", programlist->program.stdout_log.logfile);
			return EXIT_FAILURE;
		}

		if (openlogger(&programlist->program.stderr_log))
		{
			usage("could not write stderr log file %s", programlist->program.stderr_log.logfile);
			return EXIT_FAILURE;
		}

		programlist = programlist->next;
	}

	return EXIT_SUCCESS;
}

void freeconfiguration(Configuration *configuration)
{
	freeprogramlist(configuration->programs);
	freelogger(configuration->log);
	free(configuration);
}
