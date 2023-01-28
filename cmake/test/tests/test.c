#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "test.h"
#ifndef PR_SET_PTRACER
# define PR_SET_PTRACER 0x59616d61
#endif



static s32 is_atty = 0;

extern s32 fd_leak_check_enabled;

extern s32 timeouts_enabled;

extern const struct test __start_test_section, __stop_test_section;

static void usage(const char *name, s32 status)
{
	const struct test *t;

	fprintf(stderr, "Usage: %s [TEST]\n\n"
		"With no arguments, run all test.  Specify test case to run\n"
		"only that test without forking.  Available tests:\n\n",
		name);

	for (t = &__start_test_section; t < &__stop_test_section; t++)
		fprintf(stderr, "  %s\n", t->name);

	fprintf(stderr, "\n");

	exit(status);
}

#define RED	"\033[31m"
#define GREEN	"\033[32m"

static void stderr_set_color(const char *color)
{
	/* use colors only when the output is connected to
	 * the terminal */
	if (is_atty)
		fprintf(stderr, "%s", color);
}

static void stderr_reset_color(void)
{
	if (is_atty)
		fprintf(stderr, "\033[0m");
}

static const struct test *find_test(const char *name)
{
	const struct test *t;

	for (t = &__start_test_section; t < &__stop_test_section; t++)
		if (strcmp(t->name, name) == 0)
			return t;

	return NULL;
}

static void sigalrm_handler(int signum)
{
	fprintf(stderr, "Test timed out.\n");
	abort();
}

static void run_test(const struct test *t)
{
	s32 cur_fds;
	struct sigaction sa;

	if (timeouts_enabled) {
		sa.sa_handler = sigalrm_handler;
		sa.sa_flags = 0;
		sigemptyset(&sa.sa_mask);
		assert(sigaction(SIGALRM, &sa, NULL) == 0);
	}

	cur_fds = count_open_fds();

	t->run();

	/* turn off timeout (if any) after test completion */
	if (timeouts_enabled)
		alarm(0);

	check_fd_leaks(cur_fds);

	exit(EXIT_SUCCESS);
}

/* this function is taken from libinput/test/litest.c
 * (rev 028513a0a723e97941c39)
 *
 * Returns: 1 if a debugger is confirmed present; 0 if no debugger is
 * present or if it can't be determined.
 */
static int
is_debugger_attached(void)
{
	s32 status;
	s32 ret;
	pid_t pid;
	s32 pipefd[2];

	if (pipe(pipefd) == -1) {
		perror("pipe");
		return 0;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return 0;
	} else if (pid == 0) {
		char buf;
		pid_t ppid = getppid();

		/* Wait until parent is ready */
		close(pipefd[1]);  /* Close unused write end */
		ret = read(pipefd[0], &buf, 1);
		if (ret != 1)
			_exit(1);
		close(pipefd[0]);
		if (buf == '-')
			_exit(1);
		if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) != 0)
			_exit(1);
		if (!waitpid(-1, NULL, 0))
			_exit(1);
		ptrace(PTRACE_CONT, NULL, NULL);
		ptrace(PTRACE_DETACH, ppid, NULL, NULL);
		_exit(0);
	} else {
		close(pipefd[0]);

		/* Enable child to ptrace the parent process */
		ret = prctl(PR_SET_PTRACER, pid);
		if (ret != 0 && errno != EINVAL) {
			/* An error prevents us from telling if a debugger is attached.
			 * Instead of propagating the error, assume no debugger present.
			 * But note the error to the log as a clue for troubleshooting.
			 * Then flag the error state to the client by sending '-'.
			 */
			perror("prctl");
			ret = write(pipefd[1], "-", 1);
			if (ret != 1)
				_exit(1);
		} else {
			/* Signal to client that parent is ready by passing '+' */
			ret = write(pipefd[1], "+", 1);
			if (ret != 1)
				_exit(1);
		}
		close(pipefd[1]);

		waitpid(pid, &status, 0);
		ret = WEXITSTATUS(status);
	}

	return ret;
}

s32 main(s32 argc, char **argv)
{
	const struct test *t;
	s32 total, pass;
	siginfo_t info;
	pid_t pid;

	if (isatty(fileno(stderr)))
		is_atty = 1;

	if (is_debugger_attached()) {
		fd_leak_check_enabled = 0;
		timeouts_enabled = 0;
	}

	if (argc == 2 && strcmp(argv[1], "--help") == 0)
		usage(argv[0], EXIT_SUCCESS);

	if (argc == 2) {
		t = find_test(argv[1]);
		if (t == NULL) {
			fprintf(stderr, "unknown test: \"%s\"\n", argv[1]);
			usage(argv[0], EXIT_FAILURE);
		}

		run_test(t);
	}

	pass = 0;
	for (t = &__start_test_section; t < &__stop_test_section; t++) {
		s32 success = 0;

		pid = fork();
		assert(pid >= 0);

		if (pid == 0)
			run_test(t); /* never returns */

		if (waitid(P_PID, pid, &info, WEXITED)) {
			stderr_set_color(RED);
			fprintf(stderr, "waitid failed: %s\n",
				strerror(errno));
			stderr_reset_color();

			abort();
		}

		switch (info.si_code) {
		case CLD_EXITED:
			if (info.si_status == EXIT_SUCCESS)
				success = !t->must_fail;
			else
				success = t->must_fail;

			stderr_set_color(success ? GREEN : RED);
			fprintf(stderr, "test \"%s\":\texit status %d",
				t->name, info.si_status);

			break;
		case CLD_KILLED:
		case CLD_DUMPED:
			if (t->must_fail)
				success = 1;

			stderr_set_color(success ? GREEN : RED);
			fprintf(stderr, "test \"%s\":\tsignal %d",
				t->name, info.si_status);

			break;
		}

		if (success) {
			pass++;
			fprintf(stderr, ", pass.\n");
		} else
			fprintf(stderr, ", fail.\n");

		stderr_reset_color();

		/* print separator line */
		fprintf(stderr, "----------------------------------------\n");
	}

	total = &__stop_test_section - &__start_test_section;
	fprintf(stderr, "%d tests, %d pass, %d fail\n",
		total, pass, total - pass);

	return pass == total ? EXIT_SUCCESS : EXIT_FAILURE;
}

