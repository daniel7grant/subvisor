#ifndef _BASECONFIG_H_INCLUDED_
#define _BASECONFIG_H_INCLUDED_

#include "../logger/logger.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct BaseConfiguration
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
} BaseConfiguration;

extern BaseConfiguration createdefaultbaseconfig();

#endif
