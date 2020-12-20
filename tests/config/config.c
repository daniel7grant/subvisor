// #define _POSIX_C_SOURCE 1
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>

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

void test_setconfigvariable_sets_subvisord_section_variables()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;
	initializeblock(configuration, "subvisord", 0);

	PARSE_RETURN p;
	p = setconfigvariable(configuration, "subvisord", "pidfile", "/var/run/subvisord.pid");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/run/subvisord.pid", configuration->pidfile);
	p = setconfigvariable(configuration, "subvisord", "nodaemon", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nodaemon);
	p = setconfigvariable(configuration, "subvisord", "minfds", "999");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(999, configuration->minfds);
	p = setconfigvariable(configuration, "subvisord", "minprocs", "999");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(999, configuration->minprocs);
	p = setconfigvariable(configuration, "subvisord", "umask", "0440");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(0440, configuration->umask);
	p = setconfigvariable(configuration, "subvisord", "user", "username");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("username", configuration->user);
	p = setconfigvariable(configuration, "subvisord", "directory", "/home/subvisor");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/home/subvisor", configuration->directory);
	p = setconfigvariable(configuration, "subvisord", "strip_ansi", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->strip_ansi);
	p = setconfigvariable(configuration, "subvisord", "identifier", "subv");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("subv", configuration->identifier);
	p = setconfigvariable(configuration, "subvisord", "environment", "value");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("value", configuration->environment);
	p = setconfigvariable(configuration, "subvisord", "nocleanup", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nocleanup);
	p = setconfigvariable(configuration, "subvisord", "childlogdir", "/var/log/supervisord");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/supervisord", configuration->childlogdir);
	p = setconfigvariable(configuration, "subvisord", "logfile", "/var/log/subvisord.log");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/subvisord.log", configuration->log.logfile);
	p = setconfigvariable(configuration, "subvisord", "logfile_maxbytes", str("400MB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(419430400, configuration->log.logfile_maxbytes);
	p = setconfigvariable(configuration, "subvisord", "loglevel", "blather");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(BLATHER, configuration->log.loglevel);
	p = setconfigvariable(configuration, "subvisord", "logfile_backups", "20");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(20, configuration->log.logfile_backups);

	freeconfiguration(configuration);
}

void test_setconfigvariable_sets_subvisord_section_variables_with_supervisord()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;
	initializeblock(configuration, "supervisord", 0);

	PARSE_RETURN p;
	p = setconfigvariable(configuration, "supervisord", "pidfile", "/var/run/supervisord.pid");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/run/supervisord.pid", configuration->pidfile);
	p = setconfigvariable(configuration, "supervisord", "nodaemon", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nodaemon);
	p = setconfigvariable(configuration, "supervisord", "minfds", "999");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(999, configuration->minfds);
	p = setconfigvariable(configuration, "supervisord", "minprocs", "999");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(999, configuration->minprocs);
	p = setconfigvariable(configuration, "supervisord", "umask", "0440");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(0440, configuration->umask);
	p = setconfigvariable(configuration, "supervisord", "user", "username");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("username", configuration->user);
	p = setconfigvariable(configuration, "supervisord", "directory", "/home/subvisor");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/home/subvisor", configuration->directory);
	p = setconfigvariable(configuration, "supervisord", "strip_ansi", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->strip_ansi);
	p = setconfigvariable(configuration, "supervisord", "identifier", "subv");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("subv", configuration->identifier);
	p = setconfigvariable(configuration, "supervisord", "environment", "value");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("value", configuration->environment);
	p = setconfigvariable(configuration, "supervisord", "nocleanup", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->nocleanup);
	p = setconfigvariable(configuration, "supervisord", "childlogdir", "/var/log/supervisord");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/supervisord", configuration->childlogdir);
	p = setconfigvariable(configuration, "supervisord", "logfile", "/var/log/supervisord.log");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/supervisord.log", configuration->log.logfile);
	p = setconfigvariable(configuration, "supervisord", "logfile_maxbytes", str("400MB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(419430400, configuration->log.logfile_maxbytes);
	p = setconfigvariable(configuration, "supervisord", "loglevel", "blather");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(BLATHER, configuration->log.loglevel);
	p = setconfigvariable(configuration, "supervisord", "logfile_backups", "20");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(20, configuration->log.logfile_backups);

	freeconfiguration(configuration);
}

void test_setconfigvariable_sets_include_section_variables()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;
	initializeblock(configuration, "include", 0);

	PARSE_RETURN p;
	p = setconfigvariable(configuration, "include", "files", str("src/*.c"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->included_files.gl_pathc);
	TEST_ASSERT_EQUAL_STRING("src/subvisord.c", configuration->included_files.gl_pathv[0]);

	freeconfiguration(configuration);
}

void test_setconfigvariable_sets_program_section_variables()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;
	initializeblock(configuration, "program:prog", 0);
	TEST_ASSERT_NOT_NULL(configuration->programs);

	PARSE_RETURN p;
	p = setconfigvariable(configuration, "program:prog", "command", "sleep 100");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("sleep 100", configuration->programs->program.command);
	p = setconfigvariable(configuration, "program:prog", "process_name", "newname");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("newname", configuration->programs->program.process_name);
	p = setconfigvariable(configuration, "program:prog", "numprocs", "100");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(100, configuration->programs->program.numprocs);
	p = setconfigvariable(configuration, "program:prog", "numprocs_start", "5");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(5, configuration->programs->program.numprocs_start);
	p = setconfigvariable(configuration, "program:prog", "umask", "0440");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(0440, configuration->programs->program.umask);
	p = setconfigvariable(configuration, "program:prog", "priority", "500");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(500, configuration->programs->program.priority);
	p = setconfigvariable(configuration, "program:prog", "autostart", "false");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(0, configuration->programs->program.autostart);
	p = setconfigvariable(configuration, "program:prog", "autorestart", "false");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(FALSE, configuration->programs->program.autorestart);
	p = setconfigvariable(configuration, "program:prog", "startsecs", "10");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(10, configuration->programs->program.startsecs);
	p = setconfigvariable(configuration, "program:prog", "startretries", "10");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(10, configuration->programs->program.startretries);
	p = setconfigvariable(configuration, "program:prog", "exitcodes", str("0,10,100"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	for (int i = 0; i < MAX_EXITCODES; i++)
	{
		TEST_ASSERT_EQUAL(i == 0 || i == 10 || i == 100, configuration->programs->program.exitcodes[i]);
	}
	p = setconfigvariable(configuration, "program:prog", "stopsignal", "9");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(9, configuration->programs->program.stopsignal);
	p = setconfigvariable(configuration, "program:prog", "stopwaitsecs", "1");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stopwaitsecs);
	p = setconfigvariable(configuration, "program:prog", "stopasgroup", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stopasgroup);
	p = setconfigvariable(configuration, "program:prog", "killasgroup", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.killasgroup);
	p = setconfigvariable(configuration, "program:prog", "redirect_stderr", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.redirect_stderr);
	p = setconfigvariable(configuration, "program:prog", "user", "username");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("username", configuration->programs->program.user);
	p = setconfigvariable(configuration, "program:prog", "directory", "/home/subvisord");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/home/subvisord", configuration->programs->program.directory);
	p = setconfigvariable(configuration, "program:prog", "environment", "value");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("value", configuration->programs->program.environment);
	p = setconfigvariable(configuration, "program:prog", "stdout_logfile", "/var/log/subvisord/prog.log");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/subvisord/prog.log", configuration->programs->program.stdout_log.logfile);
	p = setconfigvariable(configuration, "program:prog", "stdout_logfile_maxbytes", str("300kB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(307200, configuration->programs->program.stdout_log.logfile_maxbytes);
	p = setconfigvariable(configuration, "program:prog", "stdout_logfile_backups", "20");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(20, configuration->programs->program.stdout_log.logfile_backups);
	p = setconfigvariable(configuration, "program:prog", "stdout_capture_maxbytes", str("33MB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(34603008, configuration->programs->program.stdout_log.capture_maxbytes);
	p = setconfigvariable(configuration, "program:prog", "stdout_syslog", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stdout_log.syslog);
	p = setconfigvariable(configuration, "program:prog", "stdout_events_enabled", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stdout_log.events_enabled);
	p = setconfigvariable(configuration, "program:prog", "stderr_logfile", "/var/log/subvisord/error.log");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_STRING("/var/log/subvisord/error.log", configuration->programs->program.stderr_log.logfile);
	p = setconfigvariable(configuration, "program:prog", "stderr_logfile_maxbytes", str("44kB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(45056, configuration->programs->program.stderr_log.logfile_maxbytes);
	p = setconfigvariable(configuration, "program:prog", "stderr_logfile_backups", "20");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(20, configuration->programs->program.stderr_log.logfile_backups);
	p = setconfigvariable(configuration, "program:prog", "stderr_capture_maxbytes", str("96MB"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL_UINT64(100663296, configuration->programs->program.stderr_log.capture_maxbytes);
	p = setconfigvariable(configuration, "program:prog", "stderr_syslog", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stderr_log.syslog);
	p = setconfigvariable(configuration, "program:prog", "stderr_events_enabled", "true");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);
	TEST_ASSERT_EQUAL(1, configuration->programs->program.stderr_log.events_enabled);

	// Silently fail for serverurl
	p = setconfigvariable(configuration, "program:prog", "serverurl", "");
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, p);

	freeconfiguration(configuration);
}

void test_setconfigvariable_fails_silently_for_not_added_sections()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "fcgi-program", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "eventlistener", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "rpcinterface", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "group", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "unix_http_server", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "inet_http_server", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_SUCCESS, setconfigvariable(configuration, "supervisorctl", "key", "value"));

	freeconfiguration(configuration);
}

void test_setconfigvariable_fails_for_unknown_keys()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_KEY, setconfigvariable(configuration, "subvisord", "randomkey", "100"));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_KEY, setconfigvariable(configuration, "include", "filet", "value"));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_KEY, setconfigvariable(configuration, "program:prog", "key", "100"));

	freeconfiguration(configuration);
}

void test_setconfigvariable_fails_for_unknown_sections()
{
	Configuration *configuration = (Configuration *)malloc(sizeof(Configuration));
	configuration->programs = NULL;

	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, setconfigvariable(configuration, "program", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, setconfigvariable(configuration, "porgram:typo", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, setconfigvariable(configuration, "iguana", "key", "value"));
	TEST_ASSERT_EQUAL(PARSE_ERROR_BAD_BLOCK, setconfigvariable(configuration, "2222", "key", "value"));

	freeconfiguration(configuration);
}

void test_checkfiles_open_configuration_file_if_exists()
{
	const char *configurationfile = randomstr("/tmp/tmp.");
	creat(configurationfile, 0600);

	const char *configurations[] = {};
	errno = 0;
	FILE *file = checkfiles(configurationfile, configurations, 0);
	TEST_ASSERT_NOT_NULL(file);
	TEST_ASSERT_EQUAL(0, errno);

	unlink(configurationfile);
}

void test_checkfiles_fails_if_configuration_file_not_exists()
{
	const char *configurationfile = randomstr("/tmp/tmp.");

	const char *configurations[] = {randomstr("/tmp/tmp.")};
	creat(configurations[0], 0600);
	errno = 0;
	FILE *file = checkfiles(configurationfile, configurations, 1);
	TEST_ASSERT_NULL(file);
	TEST_ASSERT_EQUAL(2, errno);
}

void test_checkfiles_open_first_existing_configuration_file()
{
	char *configurationfile = NULL;

	const char *configurations[3] = {
		randomstr("/tmp/tmp."),
		randomstr("/tmp/tmp."),
		randomstr("/tmp/tmp.")};

	creat(configurations[1], 0600);
	errno = 0;
	FILE *file = checkfiles(configurationfile, configurations, 3);
	TEST_ASSERT_NOT_NULL(file);

	unlink(configurations[1]);
}

void test_checkfiles_fails_if_default_configuration_file_not_exists()
{
	char *configurationfile = NULL;

	const char *configurations[3] = {
		randomstr("/tmp/tmp."),
		randomstr("/tmp/tmp."),
		randomstr("/tmp/tmp.")};

	errno = 0;
	FILE *file = checkfiles(configurationfile, configurations, 3);
	TEST_ASSERT_NULL(file);
	TEST_ASSERT_EQUAL(2, errno);
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
	RUN_TEST(test_setconfigvariable_sets_subvisord_section_variables);
	RUN_TEST(test_setconfigvariable_sets_subvisord_section_variables_with_supervisord);
	RUN_TEST(test_setconfigvariable_sets_include_section_variables);
	RUN_TEST(test_setconfigvariable_sets_program_section_variables);
	RUN_TEST(test_setconfigvariable_fails_silently_for_not_added_sections);
	RUN_TEST(test_setconfigvariable_fails_for_unknown_keys);
	RUN_TEST(test_setconfigvariable_fails_for_unknown_sections);
	RUN_TEST(test_checkfiles_open_configuration_file_if_exists);
	RUN_TEST(test_checkfiles_fails_if_configuration_file_not_exists);
	RUN_TEST(test_checkfiles_open_first_existing_configuration_file);
	RUN_TEST(test_checkfiles_fails_if_default_configuration_file_not_exists);
}
