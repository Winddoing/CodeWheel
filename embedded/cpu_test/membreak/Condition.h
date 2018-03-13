#ifndef _CONDITION_H_
#define _CONDITION_H_
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
# include <pthread.h>
#include "Mutex.h"
typedef long long nsecs_t;
class Condition {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    enum WakeUpType {
        WAKE_UP_ONE = 0,
        WAKE_UP_ALL = 1
    };

    Condition();
    Condition(int type);
    ~Condition();
    // Wait on the condition variable.  Lock the mutex before calling.
    int wait(Mutex& mutex);
    // same with relative timeout
    int waitRelative(Mutex& mutex, nsecs_t reltime);
    // Signal the condition variable, allowing exactly one thread to continue.
    void signal();
    // Signal the condition variable, allowing one or all threads to continue.
    void signal(WakeUpType type) {
        if (type == WAKE_UP_ONE) {
            signal();
        } else {
            broadcast();
        }
    }
    // Signal the condition variable, allowing all threads to continue.
    void broadcast();

private:
    pthread_cond_t mCond;
};
inline Condition::Condition() {
    pthread_cond_init(&mCond, NULL);
}
inline Condition::Condition(int type) {
    if (type == SHARED) {
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&mCond, &attr);
        pthread_condattr_destroy(&attr);
    } else {
        pthread_cond_init(&mCond, NULL);
    }
}
inline Condition::~Condition() {
    pthread_cond_destroy(&mCond);
}
inline int Condition::wait(Mutex& mutex) {
    return -pthread_cond_wait(&mCond, &mutex.mMutex);
}
inline int Condition::waitRelative(Mutex& mutex, nsecs_t reltime) {
    struct timespec ts;
    // we don't support the clocks here.
    struct timeval t;
    gettimeofday(&t, NULL);
    ts.tv_sec = t.tv_sec;
    ts.tv_nsec= t.tv_usec*1000;
    ts.tv_sec += reltime/1000000000;
    ts.tv_nsec+= reltime%1000000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec  += 1;
    }
    return -pthread_cond_timedwait(&mCond, &mutex.mMutex, &ts);
}
inline void Condition::signal() {
    /*
     * POSIX says pthread_cond_signal wakes up "one or more" waiting threads.
     * However bionic follows the glibc guarantee which wakes up "exactly one"
     * waiting thread.
     *
     * man 3 pthread_cond_signal
     *   pthread_cond_signal restarts one of the threads that are waiting on
     *   the condition variable cond. If no threads are waiting on cond,
     *   nothing happens. If several threads are waiting on cond, exactly one
     *   is restarted, but it is not specified which.
     */
    pthread_cond_signal(&mCond);
}
inline void Condition::broadcast() {
    pthread_cond_broadcast(&mCond);
}
#endif /* _CONDITION_H_ */
