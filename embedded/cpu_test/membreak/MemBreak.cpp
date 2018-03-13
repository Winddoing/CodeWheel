#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Thread.h"
#include <vector>
#include <algorithm>
using namespace std;
class ThreadAlloc:public Thread
{
public:
	ThreadAlloc(){
		if(AllocSizes.size() == 0){
			for(int i = 4096;i <= 4096*1024;i = i<<1){
				AllocSizes.push_back(i);
			}
			random_shuffle(AllocSizes.begin(),AllocSizes.end());
		}
	}
private:
	virtual bool        threadLoop()
	{
		srand(time(NULL));
		for(unsigned int n = 0;n < AllocSizes.size();n++){
			char *buf;
			unsigned int r = rand() * 31415926 + 12345;
			unsigned int max = AllocSizes.size();
			r = (r / (max * 2)) % max;
			int size = AllocSizes[r];
			try{
				buf = (char*)malloc(size);
				for(int i = 0; i < size;i+=4096){
					buf[i] = 0;
				}
				free(buf);
			}catch(...){
				printf("mem alloc failed!\n");
			}
		}
		return true;
	}
	static vector<int> AllocSizes;
};
vector<int> ThreadAlloc::AllocSizes;
#define THREAD_COUNT 8
int main(int argc, char *argv[])
{
	ThreadAlloc *at[THREAD_COUNT];
	for(int i = 0;i < THREAD_COUNT;i++){
		at[i] = new ThreadAlloc();
		at[i]->run();
	}
	for(int i = 0;i < THREAD_COUNT;i++){
		at[i]->join();
		delete at[i];
	}
    return 0;
}
