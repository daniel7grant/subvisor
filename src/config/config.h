#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <errno.h>
#include <glob.h>
#include <stdlib.h>
#include <string.h>

#include "configtypes.h"
#include "programconfig.h"
#include "../utils/platform.h"
#include "../logger/logger.h"

typedef enum PARSE_RETURN {
	PARSE_SUCCESS = 0,
	PARSE_SUCCESS_NEW_BLOCK,
	PARSE_SUCCESS_NEW_KEY,
	PARSE_ERROR,
	PARSE_ERROR_BAD_BLOCK,
	PARSE_ERROR_BAD_KEY,
	PARSE_ERROR_INCLUDE,
	PARSE_ERROR_BOOLEAN,
	PARSE_ERROR_NUMBER,
	PARSE_ERROR_BYTE,
	PARSE_ERROR_LOGLEVEL,
	PARSE_ERROR_EXITCODE,
	PARSE_ERROR_AUTORESTART,
} PARSE_RETURN;

typedef struct Configuration
{
	Logger log;
	char pidfile[MAX_LINE_LENGTH];
	int subvisord;
	int nodaemon;
	int minfds;
	int minprocs;
	int umask;
	char user[MAX_USERNAME_LENGTH];
	char identifier[MAX_LINE_LENGTH];
	char directory[MAX_LINE_LENGTH];
	int nocleanup;
	char childlogdir[MAX_LINE_LENGTH];
	int strip_ansi;
	char environment[MAX_LINE_LENGTH];
	glob_t included_files;
	ProgramList *programs;
} Configuration;

extern Configuration *readfromfile(FILE *filename, char **arguments);
int parsefromfile(Configuration *configuration, FILE *conffile, char *conffilename, int included);
int parsefromargs(Configuration *configuration, char **arguments);
extern int validateconfiguration(Configuration *configuration);
extern FILE *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length);
extern void freeconfiguration(Configuration *configuration);

#endif