#ifndef _THREAD_H_
#define _THREAD_H_
#include "Mutex.h"
#include "Condition.h"
typedef void* android_thread_id_t;
typedef int (*android_thread_func_t)(void*);
typedef android_thread_id_t thread_id_t;
typedef android_thread_func_t thread_func_t;
#define NO_ERROR 0
#define UNKNOWN_ERROR  -1
#define INVALID_OPERATION -2
#define WOULD_BLOCK -3
class Thread
{
public:
    // Create a Thread object, but doesn't create or start the associated
    // thread. See the run() method.
	Thread();
    virtual             ~Thread();

    // Start the thread in threadLoop() which needs to be implemented.
    virtual int    run(    const char* name = 0,
                                int32_t priority = 0,
                                size_t stack = 0);

    // Ask this object's thread to exit. This function is asynchronous, when the
    // function returns the thread might still be running. Of course, this
    // function can be called from a different thread.
    virtual void        requestExit();

    // Good place to do one-time initializations
    virtual int    readyToRun();

    // Call requestExit() and wait until this object's thread exits.
    // BE VERY CAREFUL of deadlocks. In particular, it would be silly to call
    // this function from this object's thread. Will return WOULD_BLOCK in
    // that case.
	int    requestExitAndWait();

    // Wait until this object's thread exits. Returns immediately if not yet running.
    // Do not call from this object's thread; will return WOULD_BLOCK in that case.
	int    join();

    // Indicates whether this thread is running or not.
	bool        isRunning() const;


protected:
    // exitPending() returns true if requestExit() has been called.
	bool        exitPending() const;

private:
    // Derived class must implement threadLoop(). The thread starts its life
    // here. There are two ways of using the Thread object:
    // 1) loop: if threadLoop() returns true, it will be called again if
    //          requestExit() wasn't called.
    // 2) once: if threadLoop() returns false, the thread will exit upon return.
    virtual bool        threadLoop() = 0;

private:
    Thread& operator=(const Thread&);
    static  int             _threadLoop(void* user);

    // always hold mLock when reading or writing
	thread_id_t     mThread;
	mutable Mutex           mLock;
	Condition       mThreadExitedCondition;

	int        mStatus;
    // note that all accesses of mExitPending and mRunning need to hold mLock
    volatile bool           mExitPending;
    volatile bool           mRunning;

};


#endif /* _THREAD_H_ */
