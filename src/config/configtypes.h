#ifndef _CONFIGTYPES_H_INCLUDED_
#define _CONFIGTYPES_H_INCLUDED_

#include <errno.h>
#include <string.h>

#include "../logger/logger.h"
#include "programconfig.h"

extern int toboolean(char *var);
extern int tonumber(char *var, int base);
extern long long int tobyte(char *var);
extern LOGLEVEL tologlevel(char *var);
extern int toexitcode(char *var);
extern PROGRAM_AUTORESTART toautorestart(char *var);

#endif