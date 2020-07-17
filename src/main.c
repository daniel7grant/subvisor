#include <stdio.h>

#include "config/config.h"

int main(int argv, char **argc)
{
	Configuration *configuration = readfromfile("supervisord.conf");
	printf(configuration->programs->program.user);
	return 0;
}