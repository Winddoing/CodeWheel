#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

/***********************************************
 *
 * Initialize a mutex with attribute(can be NULL)
 * int pthread_mutex_init(
 *      pthread_mutex_t* mutex,
 *      const pthread_mutexattr_t* mutexattr);
 *
 * lock a mutex
 * int pthread_mutex_lock(pthread_mutex_t* mutex);
 *
 * unlock a mutex
 * int pthread_mutex_unlock(pthread_mutex_t* mutex);
 *
 * destroy a mutex
 * int pthread_mutex_destroy(pthread_mutex_t* mutex);
 *
 * int pthread_mutexattr_setpshared(
 *       pthread_mutexattr_t* mattr,
 *       int pshared  //PTHREAD_PROCESS_SHARE | PTHREAD_PROCESS_PRIVATE
 *       );
 *
 * int pthread_mutexattr_getshared(
 * pthread_mutexattr_t* mattr,
 * int* pshared);
 *
 * int pthread_mutexattr_settype(
 *         pthread_mutexattr_t* attr,
 *         int type  //PTHREAD_MUTEX_TIMED_NP -- default value
 *                   //PTHREAD_MUTEX_RECURISIVE_NP -- allow a thread lock multitimes
 *                   //PTHREAD_MUTEX_ERRORCHECK_NO -- check error lock, return EDEADLK if the same thread want to LOCK
 *                   //PTHREAD_MUTEX_ADAPTIVE_NO -- adaptive lock, the simplest lock
 * )
 *
 *
 * int pthread_mutexattr_gettype(
 *         pthread_mutexattr_t* attr,
 *         int* type
 * )
 * *********************************************/

unsigned int gcnt = 0;
unsigned int wcnt = 0;
unsigned int RST= 0;
unsigned int RSTCNT= 0;

void* work_thread(void* p)
{
	if (NULL == p)
		return  const_cast<char*>("invalid thread argument");

	pthread_mutex_t* pMutex = (pthread_mutex_t*)(p);

	int i = 0;
	while(++i <= wcnt)
	{
		//lock multi times
		pthread_mutex_lock(pMutex);

		gcnt++;
		//cout << gcnt << endl;

		//and so unlock multi times
		pthread_mutex_unlock(pMutex);
	}
	RSTCNT += ( gcnt==RST );
	return NULL;
}

#if 0
void* work_thread2(void* p)
{
	if (NULL == p)
		return  const_cast<char*>("invalid thread argument");

	pthread_mutex_t* pMutex = (pthread_mutex_t*)(p);

	int i = 0;
	while(++i <= wcnt)
	{
		//if current thread can not enter mutex,
		//and the function pthread_mutex_trylock will RETURN Immediatly
		pthread_mutex_lock(pMutex);
		cout << ++gcnt << endl;
		pthread_mutex_unlock(pMutex);

	}

	return NULL;
}
#endif

int main(int argc, char **argv)
{
	if (argc!=3) {
		cout <<"execname nThread nCountNum" << endl;
		return 1;
	}

	const size_t nThreadCount = atoi(argv[1]);
	wcnt = atoi(argv[2]);
	RST=nThreadCount*wcnt;

	pthread_t threadIDs[nThreadCount];
	int nRet = -1;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexattr;
	void* pRet = NULL; //thread return value

	//allow a thread lock multi times
	nRet = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);

	nRet = pthread_mutex_init(&mutex, &mutexattr);
	if (0 != nRet)
		return -1;

	for (size_t i = 0; i < nThreadCount; ++ i)
	{
		nRet = pthread_create(&threadIDs[i], NULL, work_thread, (void*)(&mutex));
		if (0 != nRet)
			continue;
	}

	//nRet = pthread_create(&threadIDs[nThreadCount - 1], NULL, work_thread2, (void*)(&mutex));
	//if (0 != nRet)
	//    cerr << endl << "work_thread2 created falied! " << endl;

	int scont = 0;
	for (size_t i = 0; i < nThreadCount; ++ i)
	{
		nRet = pthread_join(threadIDs[i], &pRet);
		if (0 == nRet)
		{
			scont++;
		}

	}

	pthread_mutex_destroy(&mutex);



	if(RSTCNT==1) {
		cout << "Success!" << endl;
	}
	if(scont != nThreadCount || RSTCNT!=1) {
		cout << "fail!!" << "joined_thread:" <<scont << "RST:"<< RST << endl;
		return -1;
	}

	return 0;
}
