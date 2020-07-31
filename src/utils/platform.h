#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#define MAX_LINE_LENGTH 1024

#if defined(_WIN32) || defined(WIN32)
#include "platform.win.h"
#else
#include "platform.posix.h"
#endif // _WIN32 || WIN32

extern const char *defaultconfigurationfiles[];
extern int defaultconfigurationcount;

extern char *getcurrentdirfile(char *filename);
extern int checkuser(const char *name);
extern int checkaccess(const char *path, int writeable);
extern const char *gettempdir();
extern int getprocessid();
extern int testsyslog();
extern FILE *openprocess(char *command);
int closeprocess(FILE *pipe);

#endif