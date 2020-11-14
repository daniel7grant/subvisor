#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SUBVISORD_VERSION "0.0.0"
#define DEFAULT_LOG_MAXBYTES (50 * 1024 * 1024)
#define MAX_LINE_LENGTH 1024
#define MAX_ARGUMENTS 100

extern char *arg0;

extern void usage(const char *format, ...);

#endif
