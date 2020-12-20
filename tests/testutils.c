#include "testutils.h"

char *str(const char *input)
{
	// fix segfault errors within tests
	return strdup(input);
}

char *randomstr(const char *prefix)
{
	const char *set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
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