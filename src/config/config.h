#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

#include "../utils.h"
#include "baseconfig.h"
#include "programconfig.h"

typedef struct Configuration
{
	BaseConfiguration baseconfiguration;
	ProgramList *programs;
} Configuration;

extern Configuration *readfromfile(FILE *filename, char **arguments);
extern int validateconfiguration(Configuration *configuration);
extern FILE *checkfiles(const char *configurationfile, const char *defaultconfigurationfiles[], int length);
extern void freeconfiguration(Configuration* configuration);

#endif