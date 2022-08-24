#ifndef __TEST_H__
#define __TEST_H__

#ifdef NDEBUG
#error "Tests must not be built with NDEBUG defined, they rely on assert()."
#endif

#include <stdint.h>
#include <unistd.h>

#ifndef SHORT_TYPE_DEFINE
#define SHORT_TYPE_DEFINE
typedef uint32_t u32;
typedef int32_t s32;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint64_t u64;
typedef int64_t s64;
typedef char s8;
typedef unsigned char u8;
#endif

struct test {
	const char *name;
	void (*run)(void);
	s32 must_fail;
} __attribute__ ((aligned (16)));

#define TEST(name)							\
	static void name(void);						\
									\
	const struct test test##name					\
		 __attribute__ ((used, section ("test_section"))) = {	\
		#name, name, 0						\
	};								\
									\
	static void name(void)

#define FAIL_TEST(name)							\
	static void name(void);						\
									\
	const struct test test##name					\
		 __attribute__ ((used, section ("test_section"))) = {	\
		#name, name, 1						\
	};								\
									\
	static void name(void)

/* get the fd number */
s32 count_open_fds(void);

void check_fd_leaks(s32 supposed_fds);

/*
 * set/reset the timeout in seconds. The timeout starts
 * at the point of invoking this function
 */
void test_set_timeout(u32);

/* tester uses alarm() and SIGALRM, so we can not
 * use usleep and sleep functions in tests (see 'man usleep'
 * or 'man sleep', respectively). Following functions are safe
 * to use in tests */
void test_usleep(useconds_t);

void test_sleep(u32);

#define DISABLE_LEAK_CHECKS				\
	do {						\
		extern int fd_leak_check_enabled;	\
		fd_leak_check_enabled = 0;		\
	} while (0);

#endif

