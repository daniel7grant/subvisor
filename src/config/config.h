#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

#include "baseconfig.h"
#include "programconfig.h"

typedef struct Configuration
{
	BaseConfiguration baseconfiguration;
	ProgramList *programs;
} Configuration;

extern Configuration *readfromfile(const char *filename, char **arguments);
extern const char *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length);
extern void freeconfiguration(Configuration* configuration);

#endif