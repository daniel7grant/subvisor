#include "baseconfig.h"

BaseConfiguration createdefaultbaseconfig () {
	char *cwd = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	getcwd(cwd, MAX_LINE_LENGTH);

	BaseConfiguration baseconfiguration;
	strcpy(baseconfiguration.pidfile, cwd);
	strcat(baseconfiguration.pidfile, "/supervisord.pid");
	baseconfiguration.nodaemon = 0;
	baseconfiguration.minfds = 1024;
	baseconfiguration.minprocs = 200;
	baseconfiguration.umask = 0220;
	strcpy(baseconfiguration.user, "");
	strcpy(baseconfiguration.directory, "");
	baseconfiguration.strip_ansi = 0;
	strcpy(baseconfiguration.identifier, "subvisor");
	strcpy(baseconfiguration.environment, "");
	baseconfiguration.nocleanup = 0;
	// TODO: strcpy(baseconfiguration.childlogdir, getenv("TMPDIR"));

	char *logfile = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	strcpy(logfile, cwd);
	strcat(logfile, "/supervisord.log");

	Logger log = createlogger(logfile);
	log.loglevel = 2;

	baseconfiguration.log = log;

	free(logfile);
	free(cwd);

	return baseconfiguration;
};