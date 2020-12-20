#ifndef _TESTUTILS_H_INCLUDED_
#define _TESTUTILS_H_INCLUDED_
#define TESTING

#include "../src/utils/platform.h"
#include "../src/utils/utils.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern char *str(const char *input);
extern char *randomstr(char *prefix);
extern char *createtempname();
extern char *createtempfile(char *contents);

#endif