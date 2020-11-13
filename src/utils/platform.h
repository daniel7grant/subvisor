#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

extern const char *defaultconfigurationfiles[];
extern int defaultconfigurationcount;

extern char *getcurrentdirfile(char *filename);
extern int getuserid(const char *name);
extern int getcurrentuserid();
extern int checkaccess(const char *path, int writeable);
extern int isdir(const char *path);
extern const char *gettempdir();
extern long long int getms();

#endif