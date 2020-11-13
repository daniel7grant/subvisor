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

int initializeblock(Configuration *configuration, char *block, int included)
{
	if (strcmp(block, "subvisord") == 0 || strcmp(block, "supervisord") == 0)
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

		const char *tempdir = gettempdir();
		strcpy(configuration->childlogdir, tempdir);

		char *cwdlog = getcurrentdirfile("supervisord.log");
		Logger log = createlogger(cwdlog);
		log.loglevel = 2;
		configuration->log = log;

		free(cwdpid);
		free(cwdlog);
	}
	else if (strcmp(block, "include") == 0)
	{
		if (included) {
			// CAN'T INCLUDE IN INCLUDED FILES
			return 2;
		}
	}
	else if (strncmp(block, "program", strlen("program")) == 0)
	{
		configuration->programs = pushtoprogramlist(configuration->programs, createdefaultprogramconfig(block + strlen("progrem") + 1));
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
		return 1;
	}
	return 0;
}

int setconfigvariable(Configuration *configuration, char *block, char *key, char *value)
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
				return -1;
			}
			configuration->nodaemon = boolval;
		}
		else if (strcmp(key, "minfds") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			configuration->minfds = numval;
		}
		else if (strcmp(key, "minprocs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			configuration->minprocs = numval;
		}
		else if (strcmp(key, "umask") == 0)
		{
			int numval = tonumber(value, 8);
			if (numval < 0)
			{
				return -1;
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
				return -1;
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
				return -1;
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
				return -1;
			}
			configuration->log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "loglevel") == 0)
		{
			int logval = tologlevel(value);
			if (logval < 0)
			{
				return -1;
			}
			configuration->log.loglevel = logval;
		}
		else if (strcmp(key, "logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			configuration->log.logfile_backups = numval;
		}
		else
		{
			return 1;
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
			return 1;
		}
	}
	else if (strncmp(block, "program", strlen("program")) == 0)
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
				return -1;
			}
			programlist->program.numprocs = numval;
		}
		else if (strcmp(key, "numprocs_start") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.numprocs_start = numval;
		}
		else if (strcmp(key, "umask") == 0)
		{
			int numval = tonumber(value, 8);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.umask = numval;
		}
		else if (strcmp(key, "priority") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.priority = numval;
		}
		else if (strcmp(key, "autostart") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
			}
			programlist->program.autostart = boolval;
		}
		else if (strcmp(key, "autorestart") == 0)
		{
			int restartval = toautorestart(value);
			if (restartval < 0)
			{
				return -1;
			}
			programlist->program.autorestart = restartval;
		}
		else if (strcmp(key, "startsecs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.startsecs = numval;
		}
		else if (strcmp(key, "startretries") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.startretries = numval;
		}
		else if (strcmp(key, "exitcodes") == 0)
		{
			int exitcodes = toexitcode(value);
			if (exitcodes < 0)
			{
				return -1;
			}
			programlist->program.exitcodes = exitcodes;
		}
		else if (strcmp(key, "stopsignal") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.stopsignal = numval;
		}
		else if (strcmp(key, "stopwaitsecs") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.stopwaitsecs = numval;
		}
		else if (strcmp(key, "stopasgroup") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
			}
			programlist->program.stopasgroup = boolval;
		}
		else if (strcmp(key, "killasgroup") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
			}
			programlist->program.killasgroup = boolval;
		}
		else if (strcmp(key, "redirect_stderr") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
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
				return -1;
			}
			programlist->program.stdout_log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "stdout_logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.stdout_log.logfile_backups = numval;
		}
		else if (strcmp(key, "stdout_capture_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return -1;
			}
			programlist->program.stdout_log.capture_maxbytes = byteval;
		}
		else if (strcmp(key, "stdout_syslog") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
			}
			programlist->program.stdout_log.syslog = boolval;
		}
		else if (strcmp(key, "stdout_events_enabled") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
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
				return -1;
			}
			programlist->program.stderr_log.logfile_maxbytes = byteval;
		}
		else if (strcmp(key, "stderr_logfile_backups") == 0)
		{
			int numval = tonumber(value, 10);
			if (numval < 0)
			{
				return -1;
			}
			programlist->program.stderr_log.logfile_backups = numval;
		}
		else if (strcmp(key, "stderr_capture_maxbytes") == 0)
		{
			int byteval = tobyte(value);
			if (byteval < 0)
			{
				return -1;
			}
			programlist->program.stderr_log.capture_maxbytes = byteval;
		}
		else if (strcmp(key, "stderr_syslog") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
			}
			programlist->program.stderr_log.syslog = boolval;
		}
		else if (strcmp(key, "stderr_events_enabled") == 0)
		{
			int boolval = toboolean(value);
			if (boolval < 0)
			{
				return -1;
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
			return 1;
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
		return 1;
	}
	return 0;
}

int parseline(char *line, char *pair[2])
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
		return 0;
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
			return 1;
		}
		line[i] = '\0';

		pair[0] = &line[key];
		pair[1] = NULL;
		return 0;
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
		return 1;
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
	return 0;
}

int parsefromfile(Configuration *configuration, FILE *conffile, char *conffilename, int included)
{
	int parsing_error = 0;
	char *block = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	int linenumber = 1;
	char *pair[2];
	while (fgets(line, MAX_LINE_LENGTH, conffile) != NULL)
	{
		parsing_error |= parseline(line, pair);

		if (pair[0] != NULL && pair[1] == NULL)
		{
			// CREATE NEW BLOCK
			strcpy(block, pair[0]);
			parsing_error |= initializeblock(configuration, block, included);
		}
		else if (pair[0] != NULL && pair[1] != NULL)
		{
			// ADD NEW KEY-VALUE PAIR
			parsing_error |= setconfigvariable(configuration, block, pair[0], pair[1]);
		}

		if (parsing_error)
		{
			// PARSING FAILED
			if (pair[1] == NULL)
			{
				usage("configuration file contains parsing errors:\n\t[line %d]: %s", linenumber, line);
			}
			else
			{
				usage("configuration file contains parsing errors:\n\t[line %d]: %s = %s", linenumber, pair[0], pair[1]);
			}
			free(block);
			free(line);
			return 1;
		}

		++linenumber;
	}

	free(block);
	free(line);

	return 0;
}

int parsefromargs(Configuration *configuration, char **arguments)
{
	char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));

	// MERGE WITH ARGUMENTS FROM THE COMMAND LINE
	int k = 0;
	while (arguments[k] != NULL)
	{
		char *pair[2];
		strcpy(line, arguments[k]);
		if (parseline(line, pair))
		{
			// CONFIGURATION FAILED
			usage("command line configuration failed:\n\t%s", line);
			free(line);
			return 1;
		}

		if (pair[0] != 0 && pair[1] != 0)
		{
			// ADD NEW KEY-VALUE PAIR
			setconfigvariable(configuration, "subvisord", pair[0], pair[1]);
		}
		++k;
	}

	free(line);
	return 0;
}

int validateconfiguration(Configuration *configuration)
{
	if (!configuration->subvisord)
	{
		usage("%s", ".ini file does not include supervisord section");
		return EXIT_FAILURE;
	}

	if (openlogger(&configuration->log))
	{
		usage("could not write log file %s", configuration->log.logfile);
		return EXIT_FAILURE;
	}

	if (strlen(configuration->childlogdir) > 0 && !checkaccess(configuration->childlogdir, 0))
	{
		usage("could not access directory %s", configuration->childlogdir);
		return EXIT_FAILURE;
	}

	if (strlen(configuration->directory) > 0 && !checkaccess(configuration->directory, 0))
	{
		usage("could not access directory %s", configuration->directory);
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
			usage("user %s does not exist", configuration->user);
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
			usage("could not access directory %s", programlist->program.directory);
			return EXIT_FAILURE;
		}

		if (strlen(programlist->program.user) > 0 && getuserid(programlist->program.user) < 0)
		{
			usage("user %s does not exist", programlist->program.user);
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

void freeconfiguration(Configuration *configuration)
{
	freeprogramlist(configuration->programs);
	freelogger(configuration->log);
	free(configuration);
}
