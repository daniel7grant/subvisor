#ifndef _ARGUMENTS_H_INCLUDED_
#define _ARGUMENTS_H_INCLUDED_

#include <argp.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGUMENTS 100
#define MAX_LINE_LENGTH 1024

static struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Produce verbose output", 0},
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

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif
