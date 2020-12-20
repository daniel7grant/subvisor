#include "testutils.h"

char *str(const char *input)
{
	// fix segfault errors within tests
	return strdup(input);
}

char *randomstr(char *prefix)
{
	char *set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int prelen = strlen(prefix);
	int setlen = strlen(set);
	char *s = (char *)malloc((prelen + 10 + 1) * sizeof(char));
	strcpy(s, prefix);
	for (int i = prelen; i < prelen + 10; ++i)
	{
		s[i] = set[rand() % setlen];
	}
	s[prelen + 10] = '\0';
	return s;
}

char *createtempname()
{
	const char *tempdir = gettempdir();
	char *tempprefix = malloc((strlen(tempdir) + strlen("/tmp.") + 1) * sizeof(char));
	memset(tempprefix, 0, (strlen(tempdir) + strlen("/tmp.") + 1));
	strcpy(tempprefix, tempdir);
	strcat(tempprefix, "/tmp.");

	return randomstr(tempprefix);
}

char *createtempfile(char *contents)
{
	char *tempfile = createtempname();
	int fd = creat(tempfile, 0600);
	write(fd, contents, strlen(contents));
	close(fd);
	return tempfile;
}