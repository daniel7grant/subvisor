#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINE_LENGTH 1024

extern char *arg0;

extern void usage(const char *format, ...);

#endif
