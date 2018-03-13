#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <stack>
#include "Mutex.h"
using namespace std;
class Exception{
	typedef stack<Exception*> ExceptionStack;
	typedef map<pid_t,ExceptionStack*> ExceptionMap;
private:
	static Mutex mapLock;
	sigjmp_buf mJmpBuf;
	static struct sigaction	mOldAction;
	static struct sigaction mCurrentAction;
	static ExceptionMap mExceptionMap;
	static pid_t gettid();
	static void segv_handler(int sig);
	jmp_buf* getJmpBuf();
public:
	static void InstallException();
	static void unInstallException();
	Exception();
	~Exception();
};
#endif /* _EXCEPTION_H_ */
