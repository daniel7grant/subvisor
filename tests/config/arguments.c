#include "../../unity/src/unity.h"
#include "../testutils.h"
#include "../../src/config/arguments.h"

void test_handles_short_options()
{
	char* argv[] = {"subvisord", "-n"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("nodaemon=true", args->configurationlist[0]);
}

void test_handles_multiple_short_options()
{
	char* argv[] = {"subvisord", "-nk"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("nodaemon=true", args->configurationlist[0]);
	TEST_ASSERT_EQUAL_STRING("nocleanup=true", args->configurationlist[1]);
}

void test_handles_short_options_with_parameters()
{
	char* argv[] = {"subvisord", "-u", "name"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("user=name", args->configurationlist[0]);
}

void test_handles_multiple_short_options_with_parameters()
{
	char* argv[] = {"subvisord", "-nu", "name"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("nodaemon=true", args->configurationlist[0]);
	TEST_ASSERT_EQUAL_STRING("user=name", args->configurationlist[1]);
}

void test_handles_long_options()
{
	char* argv[] = {"subvisord", "--nodaemon"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("nodaemon=true", args->configurationlist[0]);
}

void test_handles_long_options_with_parameters_equals()
{
	char* argv[] = {"subvisord", str("--user=name")};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("user=name", args->configurationlist[0]);
}

void test_handles_long_options_with_parameters_space()
{
	char* argv[] = {"subvisord", "--user", "name"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NOT_NULL(args);
	TEST_ASSERT_EQUAL_STRING("user=name", args->configurationlist[0]);
}

void test_fails_with_positional_parameters()
{
	char* argv[] = {"subvisord", "positional"};
	int argc = sizeof(argv) / sizeof(char *);
	ParsedArguments *args = parsearguments(argc, argv);
	TEST_ASSERT_NULL(args);
}

void test_arguments() {
	printf("\n%s\n", "src/config/arguments");
	RUN_TEST(test_handles_short_options);
	RUN_TEST(test_handles_multiple_short_options);
	RUN_TEST(test_handles_short_options_with_parameters);
	RUN_TEST(test_handles_multiple_short_options_with_parameters);
	RUN_TEST(test_handles_long_options);
	RUN_TEST(test_handles_long_options_with_parameters_equals);
	RUN_TEST(test_handles_long_options_with_parameters_space);
	RUN_TEST(test_fails_with_positional_parameters);
}