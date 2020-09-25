#ifndef _PLATFORM_H_INCLUDED_
#define _PLATFORM_H_INCLUDED_

#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

extern const char *defaultconfigurationfiles[];
extern int defaultconfigurationcount;

extern char *getcurrentdirfile(char *filename);
extern int getuserid(const char *name);
extern int getcurrentuserid();
extern int checkaccess(const char *path, int writeable);
extern const char *gettempdir();
extern int testsyslog();

#endif