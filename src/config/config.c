#include "config.h"

int endofline(char c)
{
	return c == '\0' || c == '\r' || c == '\n' || c == '#' || c == ';';
}

int whitespace(char c)
{
	return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

int toboolean(char *var)
{
	return strcmp(var, "true") == 0 || strcmp(var, "1") == 0;
}

int tobyte(char *var)
{
	return atoi(var);
}

int tologlevel(char *var)
{
	return atoi(var);
}

int toexitcode(char *var)
{
	return atoi(var);
}

PROGRAM_AUTORESTART autorestart(char *var)
{
	if (strcmp(var, "true") == 0 || strcmp(var, "1") == 0)
	{
		return TRUE;
	}
	if (strcmp(var, "false") == 0 || strcmp(var, "0") == 0)
	{
		return FALSE;
	}
	return UNEXPECTED;
}

int initializeblock(Configuration *configuration, char *block)
{
	if (strcmp(block, "subvisord") == 0 || strcmp(block, "supervisord") == 0)
	{
		configuration->baseconfiguration = createdefaultbaseconfig();
	}
	else if (strcmp(block, "include") == 0)
	{
		// TODO: Add inclusion of files
	}
	else if (strncmp(block, "program", strlen("program")) == 0)
	{
		configuration->programs = pushtoprogramlist(configuration->programs, createdefaultprogramconfig());
	}
	else if (strncmp(block, "group", strlen("group")) == 0)
	{
		// TODO
	}
	else if (strncmp(block, "fcgi-program", strlen("fcgi-program")) == 0 ||
			 strncmp(block, "eventlistener", strlen("eventlistener")) == 0 ||
			 strncmp(block, "rpcinterface", strlen("rpcinterface")) == 0)
	{
		// MIGHT HAPPEN
	}
	else if (strcmp(block, "unix_http_server") == 0 || strcmp(block, "unix_http_server") == 0 || strcmp(block, "supervisorctl") == 0)
	{
		// WARNING: WILL NOT WORK
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
			strcpy(configuration->baseconfiguration.pidfile, value);
		}
		if (strcmp(key, "nodaemon") == 0)
		{
			configuration->baseconfiguration.nodaemon = toboolean(value);
		}
		if (strcmp(key, "minfds") == 0)
		{
			configuration->baseconfiguration.minfds = atoi(value);
		}
		if (strcmp(key, "minprocs") == 0)
		{
			configuration->baseconfiguration.minprocs = atoi(value);
		}
		if (strcmp(key, "umask") == 0)
		{
			configuration->baseconfiguration.umask = strtol(value, NULL, 8);
		}
		if (strcmp(key, "user") == 0)
		{
			strcpy(configuration->baseconfiguration.user, value);
		}
		if (strcmp(key, "directory") == 0)
		{
			strcpy(configuration->baseconfiguration.directory, value);
		}
		if (strcmp(key, "strip_ansi") == 0)
		{
			configuration->baseconfiguration.strip_ansi = toboolean(value);
		}
		if (strcmp(key, "identifier") == 0)
		{
			strcpy(configuration->baseconfiguration.identifier, value);
		}
		if (strcmp(key, "environment") == 0)
		{
			strcpy(configuration->baseconfiguration.environment, value);
		}
		if (strcmp(key, "nocleanup") == 0)
		{
			configuration->baseconfiguration.nocleanup = toboolean(value);
		}
		if (strcmp(key, "childlogdir") == 0)
		{
			strcpy(configuration->baseconfiguration.childlogdir, value);
		}
		if (strcmp(key, "logfile") == 0)
		{
			strcpy(configuration->baseconfiguration.log.logfile, value);
		}
		if (strcmp(key, "logfile_maxbytes") == 0)
		{
			configuration->baseconfiguration.log.logfile_maxbytes = tobyte(value);
		}
		if (strcmp(key, "loglevel") == 0)
		{
			configuration->baseconfiguration.log.loglevel = tologlevel(value);
		}
		if (strcmp(key, "logfile_backups") == 0)
		{
			configuration->baseconfiguration.log.logfile_backups = atoi(value);
		}
	}
	else if (strcmp(block, "include") == 0)
	{
		// TODO: Add inclusion of files
	}
	else if (strncmp(block, "program", strlen("program")) == 0)
	{
		ProgramList *programlist = peekprogramlist(configuration->programs);
		if (strcmp(key, "command") == 0)
		{
			strcpy(programlist->program.command, value);
		}
		if (strcmp(key, "process_name") == 0)
		{
			strcpy(programlist->program.process_name, value);
		}
		if (strcmp(key, "numprocs") == 0)
		{
			programlist->program.numprocs = atoi(value);
		}
		if (strcmp(key, "numprocs_start") == 0)
		{
			programlist->program.numprocs_start = atoi(value);
		}
		if (strcmp(key, "umask") == 0)
		{
			programlist->program.umask = strtol(value, NULL, 8);
		}
		if (strcmp(key, "priority") == 0)
		{
			programlist->program.priority = atoi(value);
		}
		if (strcmp(key, "autostart") == 0)
		{
			programlist->program.autostart = toboolean(value);
		}
		if (strcmp(key, "autorestart") == 0)
		{
			programlist->program.autorestart = autorestart(value);
		}
		if (strcmp(key, "startsecs") == 0)
		{
			programlist->program.startsecs = atoi(value);
		}
		if (strcmp(key, "startretries") == 0)
		{
			programlist->program.startretries = atoi(value);
		}
		if (strcmp(key, "exitcodes") == 0)
		{
			programlist->program.exitcodes = toexitcode(value);
		}
		if (strcmp(key, "stopsignal") == 0)
		{
			programlist->program.stopsignal = atoi(value);
		}
		if (strcmp(key, "stopwaitsecs") == 0)
		{
			programlist->program.stopwaitsecs = atoi(value);
		}
		if (strcmp(key, "stopasgroup") == 0)
		{
			programlist->program.stopasgroup = toboolean(value);
		}
		if (strcmp(key, "killasgroup") == 0)
		{
			programlist->program.killasgroup = toboolean(value);
		}
		if (strcmp(key, "redirect_stderr") == 0)
		{
			programlist->program.redirect_stderr = toboolean(value);
		}
		if (strcmp(key, "user") == 0)
		{
			strcpy(programlist->program.user, value);
		}
		if (strcmp(key, "directory") == 0)
		{
			strcpy(programlist->program.directory, value);
		}
		if (strcmp(key, "environment") == 0)
		{
			strcpy(programlist->program.environment, value);
		}
		if (strcmp(key, "stdout_logfile") == 0)
		{
			strcpy(programlist->program.stdout_log.logfile, value);
		}
		if (strcmp(key, "stdout_logfile_maxbytes") == 0)
		{
			programlist->program.stdout_log.logfile_maxbytes = tobyte(value);
		}
		if (strcmp(key, "stdout_logfile_backups") == 0)
		{
			programlist->program.stdout_log.logfile_backups = atoi(value);
		}
		if (strcmp(key, "stdout_capture_maxbytes") == 0)
		{
			programlist->program.stdout_log.capture_maxbytes = tobyte(value);
		}
		if (strcmp(key, "stdout_syslog") == 0)
		{
			programlist->program.stdout_log.syslog = toboolean(value);
		}
		if (strcmp(key, "stdout_events_enabled") == 0)
		{
			programlist->program.stdout_log.events_enabled = toboolean(value);
		}
		if (strcmp(key, "stderr_logfile") == 0)
		{
			strcpy(programlist->program.stderr_log.logfile, value);
		}
		if (strcmp(key, "stderr_logfile_maxbytes") == 0)
		{
			programlist->program.stderr_log.logfile_maxbytes = tobyte(value);
		}
		if (strcmp(key, "stderr_logfile_backups") == 0)
		{
			programlist->program.stderr_log.logfile_backups = atoi(value);
		}
		if (strcmp(key, "stderr_capture_maxbytes") == 0)
		{
			programlist->program.stderr_log.capture_maxbytes = tobyte(value);
		}
		if (strcmp(key, "stderr_syslog") == 0)
		{
			programlist->program.stderr_log.syslog = toboolean(value);
		}
		if (strcmp(key, "stderr_events_enabled") == 0)
		{
			programlist->program.stderr_log.events_enabled = toboolean(value);
		}
	}
	else if (strncmp(block, "group", strlen("group")) == 0)
	{
		// TODO
	}
	else if (strncmp(block, "fcgi-program", strlen("fcgi-program")) == 0 ||
			 strncmp(block, "eventlistener", strlen("eventlistener")) == 0 ||
			 strncmp(block, "rpcinterface", strlen("rpcinterface")) == 0)
	{
		// MIGHT HAPPEN
	}
	else if (strcmp(block, "unix_http_server") == 0 || strcmp(block, "unix_http_server") == 0 || strcmp(block, "supervisorctl") == 0)
	{
		// WARNING: WILL NOT WORK
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
		pair[0] = 0;
		pair[1] = 0;
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
			return 1;
		}
		line[i] = '\0';

		pair[0] = &line[key];
		pair[1] = 0;
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

Configuration *readfromfile(char *filename, char **arguments)
{
	FILE *conffile = fopen(filename, "r");
	if (conffile == NULL)
	{
		return NULL;
	}

	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));

	char *block = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	while (fgets(line, MAX_LINE_LENGTH, conffile) != NULL)
	{
		char *pair[2];
		if (parseline(line, pair))
		{
			// CONFIGURATION FAILED
			return NULL;
		}

		if (pair[0] != 0 && pair[1] == 0)
		{
			// CREATE NEW BLOCK
			strcpy(block, pair[0]);
			initializeblock(configuration, block);
		}
		else if (pair[0] != 0 && pair[1] != 0)
		{
			// ADD NEW KEY-VALUE PAIR
			setconfigvariable(configuration, block, pair[0], pair[1]);
		}
	}

	// MERGE WITH ARGUMENTS FROM THE COMMAND LINE
	strcpy(block, "subvisord");
	int k = 0;
	while (arguments[k] != NULL)
	{
		char *pair[2];
		strcpy(line, arguments[k]);
		if (parseline(line, pair))
		{
			// CONFIGURATION FAILED
			return NULL;
		}

		if (pair[0] != 0 && pair[1] != 0)
		{
			// ADD NEW KEY-VALUE PAIR
			setconfigvariable(configuration, block, pair[0], pair[1]);
		}
		++k;
	}

	free(block);
	free(line);
	fclose(conffile);

	return configuration;
}
