#include "Exception.h"
Exception::ExceptionMap Exception::mExceptionMap;
struct sigaction Exception::mOldAction;
struct sigaction Exception::mCurrentAction;
Mutex Exception::mapLock;
Exception::Exception(){
	pid_t tid = gettid();
	ExceptionMap::iterator it;
	mapLock.lock();
	it = mExceptionMap.find(tid);
	if(it == mExceptionMap.end()){
		ExceptionStack *stack = new ExceptionStack();
		stack->push(this);
		mExceptionMap.insert(pair<pid_t,ExceptionStack*>(tid,stack));
	}else{
		ExceptionStack *stack = it->second;
		stack->push(this);
	}
	mapLock.unlock();
	if(sigsetjmp(mJmpBuf,1))
	{
		throw "SIGSEGV";
	}
}

Exception::~Exception(){
	ExceptionMap::iterator it;
	pid_t tid = gettid();
	mapLock.lock();
	it = mExceptionMap.find(tid);
	if(it == mExceptionMap.end()){
		printf("ERROR: threadid %d not finded in ExceptionMap\n",tid);
	}else{
		ExceptionStack *stack = it->second;
		stack->pop();
		if(stack->empty()){
			mExceptionMap.erase(it);
			delete stack;
		}
	}
	mapLock.unlock();
}
sigjmp_buf* Exception::getJmpBuf(){
	return &mJmpBuf;
}
pid_t Exception::gettid()
{
	return syscall(SYS_gettid);
}
void Exception::InstallException(){
	memset(&mCurrentAction,0,sizeof(struct sigaction));
	mCurrentAction.sa_handler = Exception::segv_handler;
	mCurrentAction.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&mCurrentAction.sa_mask);

	sigaction (SIGSEGV, &mCurrentAction, NULL);
}
void Exception::unInstallException(){
	sigaction (SIGSEGV, &mOldAction, NULL);
}
void Exception::segv_handler(int sig)
{
	ExceptionMap::iterator it;
    pid_t tid = gettid();
	mapLock.lock();
	it = mExceptionMap.find(tid);
	if(it != mExceptionMap.end())
	{
		ExceptionStack *stack = it->second;
		Exception *e = stack->top();
		// stack->pop();
		// if(stack->empty()){
		// 	mExceptionMap.erase(it);
		// 	delete stack;
		// }
		// printf("pop tid %d\n",tid);
		mapLock.unlock();
		siglongjmp(*(e->getJmpBuf()), 1);
	}else{
		printf("Exception: SIGSEGV thread id %d pid = %d\n",tid,getpid());
		sleep(1);
		mapLock.unlock();
		_exit(-1);
	}
}
#if 0
int main(){
	Exception::InstallException();

	for(int i = 0;i < 10;i++){
		try{
			Exception f;
			unsigned int *a = 0;
			*a = 0;
		}catch(...)
		{
			cout << "2dddddddddddddd" << endl;
		}
	}
	Exception::unInstallException();
    return 0;
}
#endif
