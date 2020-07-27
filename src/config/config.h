#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

#include "programconfig.h"
#include "../utils.h"
#include "../logger/logger.h"

typedef struct Configuration
{
	Logger log;
	char pidfile[MAX_LINE_LENGTH];
	int nodaemon;
	int minfds;
	int minprocs;
	int umask;
	char user[MAX_LINE_LENGTH];
	char identifier[MAX_LINE_LENGTH];
	char directory[MAX_LINE_LENGTH];
	int nocleanup;
	char childlogdir[MAX_LINE_LENGTH];
	int strip_ansi;
	char environment[MAX_LINE_LENGTH];
	ProgramList *programs;
} Configuration;

extern Configuration *readfromfile(FILE *filename, char **arguments);
extern int validateconfiguration(Configuration *configuration);
extern FILE *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length);
extern void freeconfiguration(Configuration* configuration);

#endif