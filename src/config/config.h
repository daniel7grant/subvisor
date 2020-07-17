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

extern Configuration *readfromfile(char *filename, char **arguments);

#endif