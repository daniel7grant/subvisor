#include "../../unity/src/unity.h"
#include "../testutils.h"
#include "../../src/config/config.h"

void test_parseline_parses_empty_lines()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str(""), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str("  "), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str(" \t "), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);
}

void test_parseline_parses_comments()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str("# comment"), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str(";other comment"), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str(" # whitespace & comment "), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);
}

void test_parseline_parses_sections()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str("[section]"), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_BLOCK, p);
	TEST_ASSERT_EQUAL_STRING("section", pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str(" [section]  "), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_BLOCK, p);
	TEST_ASSERT_EQUAL_STRING("section", pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str(" [section]  # comment"), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_BLOCK, p);
	TEST_ASSERT_EQUAL_STRING("section", pair[0]);
	TEST_ASSERT_NULL(pair[1]);
}

void test_parseline_fails_if_sections_end_missing()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str("[section"), pair);
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str("[section #]"), pair);
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);
}

void test_parseline_parses_key_values()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str("key=value"), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_KEY, p);
	TEST_ASSERT_EQUAL_STRING("key", pair[0]);
	TEST_ASSERT_EQUAL_STRING("value", pair[1]);

	p = parseline(str(" key =\tvalue  "), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_KEY, p);
	TEST_ASSERT_EQUAL_STRING("key", pair[0]);
	TEST_ASSERT_EQUAL_STRING("value", pair[1]);

	p = parseline(str("key="), pair);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS_NEW_KEY, p);
	TEST_ASSERT_EQUAL_STRING("key", pair[0]);
	TEST_ASSERT_EQUAL_STRING("", pair[1]);
}

void test_parseline_fails_if_no_key_value()
{
	char *pair[2];
	PARSE_RETURN p;
	p = parseline(str("key"), pair);
	TEST_ASSERT_EQUAL(PARSE_ERROR, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);

	p = parseline(str("key #= value"), pair);
	TEST_ASSERT_EQUAL(PARSE_ERROR, p);
	TEST_ASSERT_NULL(pair[0]);
	TEST_ASSERT_NULL(pair[1]);
}

void test_initializeblock_creates_subvisord_section()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	PARSE_RETURN p;
	p = initializeblock(configuration, "subvisord", 0);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->subvisord);
	TEST_ASSERT_EQUAL_STRING("supervisord.pid", configuration->pidfile + strlen(configuration->pidfile) - strlen("supervisord.pid"));
	TEST_ASSERT_EQUAL(0, configuration->nodaemon);
	TEST_ASSERT_EQUAL(1024, configuration->minfds);
	TEST_ASSERT_EQUAL(200, configuration->minprocs);
	TEST_ASSERT_EQUAL(0220, configuration->umask);
	TEST_ASSERT_EQUAL_STRING("", configuration->user);
	TEST_ASSERT_EQUAL_STRING("", configuration->directory);
	TEST_ASSERT_EQUAL(0, configuration->strip_ansi);
	TEST_ASSERT_EQUAL_STRING("subvisor", configuration->identifier);
	TEST_ASSERT_EQUAL_STRING("", configuration->environment);
	TEST_ASSERT_EQUAL(0, configuration->nocleanup);
	TEST_ASSERT_NULL(configuration->programs);
	TEST_ASSERT_EQUAL(2, configuration->log.loglevel);
	TEST_ASSERT_EQUAL_STRING("supervisord.log", configuration->log.logfile + strlen(configuration->log.logfile) - strlen("supervisord.log"));

	// Test that multiple addition doesn't reset section
	configuration->nodaemon = 1;
	p = initializeblock(configuration, "subvisord", 0);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nodaemon);

	freeconfiguration(configuration);
}

void test_initializeblock_creates_subvisord_section_with_supervisord()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	PARSE_RETURN p;
	p = initializeblock(configuration, "supervisord", 0);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->subvisord);

	// Test that multiple addition doesn't reset section
	configuration->nodaemon = 1;
	p = initializeblock(configuration, "subvisord", 0);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nodaemon);
	
	freeconfiguration(configuration);
}

void test_initializeblock_creates_include_section_if_not_included()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "include", 0));
	TEST_ASSERT_EQUAL(PARSE_ERROR_INCLUDE, initializeblock(configuration, "include", 1));
	
	freeconfiguration(configuration);
}

void test_initializeblock_creates_default_program_section()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	PARSE_RETURN p;
	p = initializeblock(configuration, str("program:prog"), 0);
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_NOT_NULL(configuration->programs);
	TEST_ASSERT_EQUAL_STRING("", configuration->programs->program.command);
	TEST_ASSERT_EQUAL_STRING("prog", configuration->programs->program.process_name);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.numprocs);
	TEST_ASSERT_EQUAL(0, configuration->programs->program.numprocs_start);
	TEST_ASSERT_EQUAL(-1, configuration->programs->program.umask);
	TEST_ASSERT_EQUAL(999, configuration->programs->program.priority);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.autostart);
	TEST_ASSERT_EQUAL(UNEXPECTED, configuration->programs->program.autorestart);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.startsecs);
	TEST_ASSERT_EQUAL(3, configuration->programs->program.startretries);
	for (int i = 0; i < MAX_EXITCODES; i++)
	{
		TEST_ASSERT_EQUAL(i == 0, configuration->programs->program.exitcodes[i]);
	}
	TEST_ASSERT_EQUAL(0, configuration->programs->program.stopsignal);
	TEST_ASSERT_EQUAL(10, configuration->programs->program.stopwaitsecs);
	TEST_ASSERT_EQUAL(0, configuration->programs->program.stopasgroup);
	TEST_ASSERT_EQUAL(0, configuration->programs->program.killasgroup);
	TEST_ASSERT_EQUAL(0, configuration->programs->program.redirect_stderr);
	TEST_ASSERT_EQUAL_STRING("", configuration->programs->program.user);
	TEST_ASSERT_EQUAL_STRING("", configuration->programs->program.directory);
	TEST_ASSERT_EQUAL_STRING("", configuration->programs->program.environment);
	
	freeconfiguration(configuration);
}

void test_initializeblock_fails_silently_for_not_added_sections()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "fcgi-program", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "eventlistener", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "rpcinterface", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "group", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "unix_http_server", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "inet_http_server", 0));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, initializeblock(configuration, "supervisorctl", 0));
	
	freeconfiguration(configuration);
}

void test_initializeblock_fails_for_unknown_sections()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, initializeblock(configuration, "program", 0));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, initializeblock(configuration, "porgram:typo", 0));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, initializeblock(configuration, "iguana", 0));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, initializeblock(configuration, "2222", 0));
	
	freeconfiguration(configuration);
}

void test_config()
{
	printf("\n%s\n", "src/config/config");
	RUN_TEST(test_parseline_parses_empty_lines);
	RUN_TEST(test_parseline_parses_comments);
	RUN_TEST(test_parseline_parses_sections);
	RUN_TEST(test_parseline_fails_if_sections_end_missing);
	RUN_TEST(test_parseline_parses_key_values);
	RUN_TEST(test_parseline_fails_if_no_key_value);
	RUN_TEST(test_initializeblock_creates_subvisord_section);
	RUN_TEST(test_initializeblock_creates_subvisord_section_with_supervisord);
	RUN_TEST(test_initializeblock_creates_include_section_if_not_included);
	RUN_TEST(test_initializeblock_creates_default_program_section);
	RUN_TEST(test_initializeblock_fails_silently_for_not_added_sections);
	RUN_TEST(test_initializeblock_fails_for_unknown_sections);
}