#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINE_LENGTH 1024

#include "baseconfig.h"
#include "programconfig.h"

extern char *arg0;

typedef struct Configuration
{
	BaseConfiguration baseconfiguration;
	ProgramList *programs;
} Configuration;

extern void usage(const char *format, ...);
extern Configuration *readfromfile(FILE *filename, char **arguments);
extern int validateconfiguration(Configuration *configuration);
extern FILE *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length);
extern void freeconfiguration(Configuration* configuration);

#endif