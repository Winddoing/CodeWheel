#ifndef __DUMPSTACK_H__
#define __DUMPSTACK_H__

#include <execinfo.h>


/* 用法：
 *	添加到程序中后，编译时需要使用-g -rdynamic参数
 *	*/

static void dump(void)
{
	void *buffer[30] = {0};
	size_t size;
	char **strings = NULL;
	size_t i = 0;

	size = backtrace(buffer, 30);

	strings = backtrace_symbols(buffer, size);
	if (strings == NULL) {
		perror("backtrace_symbols.");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Obtained %zd stack frames\n", size);
	for (i = 0; i < size; i++) {
		fprintf(stdout, "%s\n", strings[i]);
	}

	free(strings);
	exit(0);
}


#endif //__DUMPSTACK_H__
