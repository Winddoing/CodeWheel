#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#if defined(__mips__)
#include <asm/cachectl.h>
#include <sys/cachectl.h>
#endif
#include <atomic>
#include "Thread.h"
#include "Exception.h"
using namespace std;

class Instruction
{
private:
	uint32_t *mInstBuf;
    int mSize;
	volatile uint32_t compiled;
	volatile uint32_t executed;
	volatile uint32_t done;
	int mMapFlag;
public:
	Instruction(int flag);
	~Instruction();
	void BuildInst();
	void ReadExecInst();
};
#define BUFFER_SIZE 256
#if defined (__i386__)
#define FUNCTION_SIZE 1
#elif defined (__mips__)
#define FUNCTION_SIZE 2
#else
#error "Not Support this arch!"
#endif
Instruction::Instruction(int execflag)
{
	mSize = BUFFER_SIZE * FUNCTION_SIZE * 4;
	mMapFlag = execflag;
	mInstBuf = (uint32_t *)mmap(NULL, mSize, execflag, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(mInstBuf == NULL){
		printf("PPPPPPPPPPPPPPPPPPPPPPPPP\n");
		return;
	}
	compiled = 0;
	executed = 0;
	try{
		Exception e;
		for (int i = 0; i < mSize / 4; i++) {
#if defined (__i386__)
			*(mInstBuf + i) = 0xcccccccc; /* 4 * INT 3 */
#elif defined (__mips__)
			*(mInstBuf + i) = 0x0000FFF4; /* TEQ zero, zero, #3FF */
#endif
		}
	}catch(...)
	{
		if(mMapFlag | PROT_WRITE){
			fprintf(stderr,"write check:it isn't reach hear,check it. %s %d!\n",__FILE__,__LINE__);
			exit(-1);
		}
	}
	__builtin___clear_cache((char *)mInstBuf, (char *)(mInstBuf + mSize / 4));
	done = 1;
}
Instruction::~Instruction()
{
	munmap(mInstBuf,mSize);
}
void Instruction::BuildInst()
{
	if(mInstBuf == NULL){
		printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		done = 0;
		return;
	}
	for (int i = 0; i < mSize / 4; i += FUNCTION_SIZE) {
		while (executed != compiled);
		if(i == mSize /4 - FUNCTION_SIZE)
			done = 0;

		try{
			Exception e;
#if defined(__i386__)
			*(mInstBuf + i) = 0x000000c3; /* RET */
#elif defined(__mips__)
			*(mInstBuf + i) =       0x03e00008; /* JR RA */
			*(mInstBuf + i + 1)   = 0x00000000; /* NOP in delay slot */
#endif
			__builtin___clear_cache((char *)(mInstBuf + i), (char *)(mInstBuf + i + FUNCTION_SIZE));
			/* publish the method pointer */
			compiled = (uint32_t)(mInstBuf + i);
		}catch(...){
			if(mMapFlag | PROT_WRITE){
				fprintf(stderr,"write check:it isn't reach hear,check it. %s %d!\n",__FILE__,__LINE__);
				exit(-1);
			}
		}
	}
}
void Instruction::ReadExecInst()
{
	while(done){
		while(executed == compiled);
		uint32_t c = compiled;
		try{
			Exception e;
			void (*f)(void);
			f = (void (*)(void))(c);
			(*f)();

			if(!(mMapFlag & PROT_EXEC)){
				fprintf(stderr,"exec check(MAPFLAG:0x%08x):it isn't reach hear,check it. %s %d!\n",mMapFlag,__FILE__,__LINE__);
				exit(-1);
			}
		}catch(...){
			if(mMapFlag & PROT_EXEC){
				fprintf(stderr,"exec check(MAPFLAG:0x%08x):it isn't reach hear,check it. %s %d!\n",mMapFlag,__FILE__,__LINE__);
				exit(-1);
			}
		}
		try{
			Exception e;
			volatile uint32_t *d;
			d = (volatile uint32_t *)c;
			*d = *d;
		    if(!(mMapFlag & PROT_READ)){
				fprintf(stderr,"read check(MAPFLAG:0x%08x):it isn't reach hear,check it. %s %d!\n",mMapFlag,__FILE__,__LINE__);
				exit(-1);
			}
		}catch(...){
			if(mMapFlag & PROT_READ){
				fprintf(stderr,"read check(MAPFLAG:0x%08x):it isn't reach hear,check it. %s %d!\n",mMapFlag,__FILE__,__LINE__);
				exit(-1);
			}
		}
		executed = c;
	}
}

class ExecRun:public Thread{
private:
	Instruction *inst;
	virtual bool threadLoop(){
		inst->ReadExecInst();
		return false;
	}
public:
	ExecRun(Instruction *i){
		inst = i;
	}
};
class BuildRun:public Thread{
private:
	Instruction *inst;
	virtual bool threadLoop(){
		inst->BuildInst();
		return false;
	}
public:
	BuildRun(Instruction *i){
		inst = i;
	}
};
class TestRun
{
private:

	ExecRun *exec;
	BuildRun *build;
	Instruction *inst;
public:
	TestRun(int execflag)
	{
		inst = new Instruction(execflag);
		exec = new ExecRun(inst);
		build = new BuildRun(inst);
		exec->run();
		build->run();
	}
	~TestRun(){
		exec->join();
		build->join();
		delete inst;
		delete exec;
		delete build;
	}
};
int main(int argc, char *argv[])
{
	Exception::InstallException();
	printf("Test Run ...\n");
	printf("PORT_READ = 0x%08x\n",PROT_READ);
	printf("PORT_WRITE = 0x%08x\n",PROT_WRITE);
	printf("PORT_EXEC = 0x%08x\n",PROT_EXEC);

	TestRun *t1 =  new TestRun(PROT_READ | PROT_WRITE | PROT_EXEC);
	TestRun *t2 =  new TestRun(PROT_READ | PROT_WRITE);
	//TestRun *t3 =  new TestRun(PROT_EXEC | PROT_WRITE);
	delete t1;
	delete t2;
	//delete t3;
	printf("Run Ok!\n");
	Exception::unInstallException();
    return 0;
}
