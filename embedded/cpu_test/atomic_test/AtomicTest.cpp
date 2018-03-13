#include <stdio.h>
#include <string.h>
#include <atomic>
#include "Thread.h"
using namespace std;
class AtomicTest:public Thread
{
#define TEST_BUFFER      64*1024
public:
	AtomicTest(){
		count = 0;
		memset(atomic_buf,0,TEST_BUFFER * 4);
	}
	static atomic<bool> startrun;
	void DumpBuf(){
		for(int j = 0;j < TEST_BUFFER;j++){
			if(j % 8 == 0)
				printf("\n%d:%4d ",j,atomic_buf[j]);
			else
				printf("%4d ",atomic_buf[j]);
		}
		printf("\n");
	}
	int32_t getAtomicBuf(unsigned int index){
		if(index >= TEST_BUFFER)
			return -1;
		return atomic_buf[index];
	}
private:
	virtual bool        threadLoop();
	static atomic<int32_t> atomic;
	int32_t atomic_buf[TEST_BUFFER];
	int32_t count;
};
atomic<int32_t>  AtomicTest::atomic(0);
atomic<bool>  AtomicTest::startrun(false);
bool AtomicTest::threadLoop()
{
	 while(!startrun);
	atomic_buf[count++] = atomic++;
	if(count >= TEST_BUFFER)
		return false;
	return true;
}
#define THREAD_COUNT 8
int main(int argc, char *argv[])
{
	int ret = 0;
	AtomicTest *at[THREAD_COUNT];
	for(int i = 0;i < THREAD_COUNT;i++){
		at[i] = new AtomicTest();
		ret = at[i]->run();
		if (ret) {
			for(int j = 0;i < i;j++)
				delete at[j];
			return -1;
		}
	}
	AtomicTest::startrun = 1;
	for(int i = 0;i < THREAD_COUNT;i++)
		at[i]->join();
	int32_t e = 0,se = 0;
	int32_t n[THREAD_COUNT] = {0};
	int32_t errflag = 0;
	int32_t r;
	for(int i = 0;i < THREAD_COUNT;i++){
		se = e;
		for(int j = n[i];j < TEST_BUFFER;j++){
			r = at[i]->getAtomicBuf(j);
			if(r == e){
				e++;
				n[i]++;
			}else if(r > e){
				n[i] = j;
				break;
			} else {
				errflag = 1;
				printf("thread id:%d position:%d error! expect:%d really:%d !\n",i,j,e,r);
				break;
			}
		}
		if(se != e)	i = -1;
	}
	if(e != TEST_BUFFER * THREAD_COUNT){
		printf("total error! max expect:%d really:%d\n",TEST_BUFFER * THREAD_COUNT - 1,e);
		errflag = 1;
	}
	if(errflag){
		for(int i = 0;i < THREAD_COUNT;i++){
			printf("===============thread id %d================== \n",i);
			at[i]->DumpBuf();
		}
	}else {
		printf("Test OK.\n");
		for(int i = 0;i < THREAD_COUNT;i++){
			printf("===============thread id %d================== \n",i);
			int32_t start = at[i]->getAtomicBuf(0);
			int32_t end = at[i]->getAtomicBuf(TEST_BUFFER - 1);
			printf("first:0x%08x,last:0x%08x,diff:%d\n",start,end,end-start);
		}
	}

	for(int i = 0;i < THREAD_COUNT;i++){
		delete at[i];
	}
    return 0;
}
