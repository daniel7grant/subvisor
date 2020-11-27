#include "../../unity/src/unity.h"
#include "../testutils.h"
#include "../../src/config/programconfig.h"

void test_default_programconfig_values()
{
	ProgramConfiguration program = createdefaultprogramconfig("program1");
	TEST_ASSERT_EQUAL_STRING("", program.command);
	TEST_ASSERT_EQUAL_STRING("program1", program.process_name);
	TEST_ASSERT_EQUAL(1, program.numprocs);
	TEST_ASSERT_EQUAL(0, program.numprocs_start);
	TEST_ASSERT_EQUAL(-1, program.umask);
	TEST_ASSERT_EQUAL(999, program.priority);
	TEST_ASSERT_EQUAL(1, program.autostart);
	TEST_ASSERT_EQUAL(UNEXPECTED, program.autorestart);
	TEST_ASSERT_EQUAL(1, program.startsecs);
	TEST_ASSERT_EQUAL(3, program.startretries);
	for (int i = 0; i < MAX_EXITCODES; i++)
	{
		TEST_ASSERT_EQUAL(i == 0, program.exitcodes[i]);
	}
	TEST_ASSERT_EQUAL(0, program.stopsignal);
	TEST_ASSERT_EQUAL(10, program.stopwaitsecs);
	TEST_ASSERT_EQUAL(0, program.stopasgroup);
	TEST_ASSERT_EQUAL(0, program.killasgroup);
	TEST_ASSERT_EQUAL(0, program.redirect_stderr);
	TEST_ASSERT_EQUAL_STRING("", program.user);
	TEST_ASSERT_EQUAL_STRING("", program.directory);
	TEST_ASSERT_EQUAL_STRING("", program.environment);
}

void test_pushing_values_onto_programlist()
{
	ProgramConfiguration program0 = createdefaultprogramconfig("program0");
	ProgramConfiguration program1 = createdefaultprogramconfig("program1");
	ProgramConfiguration program2 = createdefaultprogramconfig("program2");
	ProgramList *programlist = NULL;
	programlist = pushtoprogramlist(programlist, program0);
	programlist = pushtoprogramlist(programlist, program1);
	programlist = pushtoprogramlist(programlist, program2);

	ProgramConfiguration programs[] = {program0, program1, program2};
	for (int i = 0; programlist->next != NULL; ++i)
	{
		TEST_ASSERT_EQUAL_STRING(programs[i].process_name, programlist->program.process_name);
		programlist = programlist->next;
	}
}

void test_reading_last_values_from_programlist()
{
	ProgramConfiguration program0 = createdefaultprogramconfig("program0");
	ProgramConfiguration program1 = createdefaultprogramconfig("program1");
	ProgramConfiguration program2 = createdefaultprogramconfig("program2");
	ProgramList *programlist = NULL;
	programlist = pushtoprogramlist(programlist, program0);
	programlist = pushtoprogramlist(programlist, program1);
	programlist = pushtoprogramlist(programlist, program2);

	TEST_ASSERT_EQUAL_STRING(program2.process_name, peekprogramlist(programlist)->program.process_name);
	freeprogramlist(programlist);
}

void test_counting_values_from_programlist()
{
	ProgramConfiguration program0 = createdefaultprogramconfig("program0");
	ProgramConfiguration program1 = createdefaultprogramconfig("program1");
	ProgramConfiguration program2 = createdefaultprogramconfig("program2");
	ProgramList *programlist = NULL;
	programlist = pushtoprogramlist(programlist, program0);
	programlist = pushtoprogramlist(programlist, program1);
	programlist = pushtoprogramlist(programlist, program2);

	TEST_ASSERT_EQUAL(3, countprogramlist(programlist));
	freeprogramlist(programlist);
}

void test_programconfig()
{
	printf("\n%s\n", "src/config/programconfig");
	RUN_TEST(test_default_programconfig_values);
	RUN_TEST(test_pushing_values_onto_programlist);
	RUN_TEST(test_reading_last_values_from_programlist);
	RUN_TEST(test_counting_values_from_programlist);
}