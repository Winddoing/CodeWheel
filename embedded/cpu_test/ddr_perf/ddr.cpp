#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <malloc.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <vector>
using namespace std;
#ifdef __mips__
extern "C" void *memset_jz(void *s, int c, size_t n);
extern "C" void *memcpy_jz(void *dest, const void *src, size_t n);
#define memset memset_jz
#define memcpy memcpy_jz

#ifdef SIMD_TEST
extern "C" void *simd_memset(void *s, int c, size_t n);
extern "C" void *simd_memcpy(void *dest, const void *src, size_t n);
#else
#define simd_memset(x,y,z)
#define simd_memcpy(x,y,z)
#endif

#else
#define simd_memset(x,y,z)
#define simd_memcpy(x,y,z)
#endif
class DebugFSOps {
private:
	string mPath;
	list<string> mFiles;
	map<string,string> mData;
public:
	DebugFSOps(char *path)
	{
		if(path){
			int len = strlen(path);
			if(len > 0)
			{
				if(path[len - 1] == '/')
					mPath = string(path);
				else
					mPath = string(path) + string("/");
			}
			if(listdir(mPath) == 0)
			{
				list<string>::iterator it;
				printf("find files:\n");
				for(it = mFiles.begin();it != mFiles.end();it++){
					cout << *it << endl;
				}
			}
		}
	}
	~DebugFSOps()
	{
	}
	void ReadSync()
	{
		list<string>::iterator it;
		mData.clear();
		for(it = mFiles.begin();it != mFiles.end();it++){
			FILE *fp;
			string path = mPath + *it;
			fp = fopen(path.c_str(),"r");
			if(fp != NULL){
				char dat[256];
				int len = fread(dat,1,256,fp);
				if(len == 256)
					printf("max value 256, data too long.\n");
				dat[len] = 0;
				mData.insert(pair<string,string>(*it,dat));
				fclose(fp);
			}
		}
	}
	const char *read_string(const char *s)
	{
		map<string,string>::iterator it;
		it = mData.find(string(s));
		if(it != mData.end())
		{
			return it->second.c_str();
		}
		return NULL;
	}
	long read_long(const char *s)
	{
		map<string,string>::iterator it;
		it = mData.find(string(s));
		if(it != mData.end())
		{
			return std::stol(it->second,0,0);
		}
		return -1;
	}
	void write(const char *s,long d)
	{
		FILE *fp;
		string p = mPath + string(s);
		fp = fopen(p.c_str(),"w+");
		if(fp != NULL)
		{
			char dat[20];
			sprintf(dat,"%ld",d);
			fwrite(dat,1,strlen(dat),fp);
			fclose(fp);
		}else{
			printf("write %s failed!\n",s);
		}
	}
private:
	int listdir (string dir)
	{
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir(dir.c_str())) == NULL) {
			cout << "Error(" << errno << ") opening " << dir << endl;
			return errno;
		}
		while ((dirp = readdir(dp)) != NULL) {
			if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,".."))
				mFiles.push_back(string(dirp->d_name));
		}
		closedir(dp);
		return 0;
	}

};
class PerfectTest {
private:
	static vector<PerfectTest*> myTests;

	unsigned long long getSystemTime()
	{
		struct timeval curtime;
		gettimeofday(&curtime,NULL);
		return curtime.tv_sec * 1000000LL + curtime.tv_usec;
	}
	unsigned int mSize;
	unsigned int mLoop;
	void *mSrc,*mTar;
	void *mOrgSrc,*mOrgTar;
	DebugFSOps *mDebugFs;
public:
	static void TestAll(){
		for (unsigned int i = 0; i < myTests.size(); ++i)
		{
			PerfectTest *test = myTests[i];
			test->TimeTest();
			if(test->mDebugFs){
				test->CycleTest();
				test->BandwidthTest();
			}
		}
	}

	PerfectTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs){
		if(size == 0){
			size = 512*1024;
		}
		mSize = (size + 4093) / 4096 * 4096;
		if(loop == 0)
			loop = 1;
		mLoop = loop;
		mTar = mSrc = NULL;
		mOrgSrc = mOrgTar = NULL;
		mDebugFs = debugfs;
		myTests.push_back(this);
	}
	~PerfectTest(){
		FreeSpace();
	}

	bool TimeTest(){
		unsigned long long t,d;
		bool check = true;
		AllocSpace();
		if(!preDDRTest(mSrc,mTar,mSize)){
			FreeSpace();
			return false;
		}
		t = getSystemTime();
		d = DDRTest(mSrc,mTar,mSize,mLoop);
		t = getSystemTime() - t;
		printf("%lld\n",d);
		if(t > 0)
			d = d * 1000000LL / t;
		else
			d = 0LL;
		check = postDDRTest(mSrc,mTar,mSize);
		printf("%lld %lld\n",d,t);
		printf("elapse time:%lldus perf:%lld.%03lldM/sec check:%s\n",
			   t,d / 1000000LL,d - d / 1000000LL * 1000000LL,
			   check?"True":"False");
		FreeSpace();
		return check;
	}
	bool CycleTest(){
		unsigned long long d;
		DebugFSOps *debugfs = mDebugFs;
		bool check = true;
		long wcycle;
		long rcycle;
		AllocSpace();
		if(!preDDRTest(mSrc,mTar,mSize)){
			FreeSpace();
			return false;
		}
		debugfs->write("output",0);
		debugfs->write("periods",-1);
		debugfs->write("run",1);
		d = DDRTest(mSrc,mTar,mSize,mLoop);
		debugfs->write("run",0);
		debugfs->ReadSync();
		rcycle = debugfs->read_long("cpu_read_rate");
		wcycle = debugfs->read_long("cpu_write_rate");
		printf("cpu really read & write %lld bytes data.\n",d);
		if(rcycle != -1)
			printf("ddr read  %ld bytes data.\n",rcycle * 32);
		if(wcycle != -1)
			printf("cpu write %ld bytes data.\n",wcycle * 32);
		if(rcycle > 0 && wcycle > 0)
			printf("cpu use ratio %0.03f\n",(float)d / (rcycle + wcycle) * 32.0f);
		check = postDDRTest(mSrc,mTar,mSize);
		printf("data check:%s\n",check?"True":"False");
		FreeSpace();
		return check;
	}
	bool BandwidthTest()
	{
		unsigned long long d;
		bool check = true;
		DebugFSOps *debugfs = mDebugFs;
		AllocSpace();
		if(!preDDRTest(mSrc,mTar,mSize)){
			FreeSpace();
			return false;
		}
		debugfs->write("output",1);
		debugfs->write("periods",mSize * mLoop / 4);
		debugfs->write("run",1);
		d = DDRTest(mSrc,mTar,mSize,mLoop);
		debugfs->write("run",0);
		check = postDDRTest(mSrc,mTar,mSize);
		printf("data test total count %lld\n",d);
		printf("data check:%s\n",check?"True":"False");
		FreeSpace();
		return check;
	}

private:
	void AllocSpace()
	{
		int size = mSize;
		mOrgSrc = malloc(size + 4096);
		mOrgTar = malloc(size + 4096);
		if(mOrgSrc == 0 || mOrgTar == 0){
			printf("buffer require failed! size = %d\n",size);
		}
#define aligned_addr(x,align) (((unsigned int)x + align + 1) / align * align)
		mSrc = (void*)aligned_addr(mOrgSrc,4096);
		mTar = (void*)aligned_addr(mOrgTar,4096);
	}
	void FreeSpace()
	{
		if(mOrgSrc)
			free(mOrgSrc);
		if(mOrgTar)
			free(mOrgTar);
	}

protected:
	bool checkBuffer(){
		int d;
		d = memcmp(mSrc,mTar,mSize);
		if(d){
			if(d < 0) d = -d;
			printf("error: src[%p + %d]:%02x tar[%p + %d]:%02x\n",
				   mSrc,d,*((unsigned char*)mSrc + d),
				   mTar,d,*((unsigned char*)mTar + d));
		}
		return d == 0;
	}
	/**
	 * @src:  test source buffer.
	 * @tar:  test target buffer.
	 * @size: test package size.
	 * @loop: test loop count.
	 * @return: test data size.
	 */
	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop) = 0;
	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		return true;
	}
	virtual bool postDDRTest(void * src,void *tar,unsigned int size)
	{
		return true;
	}
};
vector<PerfectTest*> PerfectTest::myTests;

class DDRWriteTest :public PerfectTest
{
public:
	DDRWriteTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs):PerfectTest(size,loop,debugfs){
	}
	~DDRWriteTest(){}
private:
	/*
	 * Add memset to prevent tlb load loss when it write data.
	 */
	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		printf("Write Test...\n");
		memset(src,0xaa,size);
		memset(tar,0xaa,size);
		return true;
	}

	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop)
	{
		int l = loop;
		while(l--){
			memset(src,0xaa,size);
			memset(tar,0xaa,size);
		}
		return size * 2LL * loop;
	}
};

class DDRReadTest :public PerfectTest
{
public:
	DDRReadTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs):PerfectTest(size,loop,debugfs){

	}
	~DDRReadTest(){}
private:
	/*
	 * Add memset to prevent read when kernel do not allocation address.
	 */
	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		if(size & (4096 - 1)){
			printf("test size should be 4096byte aligned.\n");
			return false;
		}
		memset(src,0xaa,size);
		memset(tar,0xaa,size);
		printf("Read Test...\n");
		return true;
	}
	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop)
	{
		int l = loop;
		while(l--){
#ifdef __mips__
#define memread(x) do{							\
				__asm__ __volatile__(			\
					"pref    0,96(%0)\n\t"		\
					"lw $0, 0(%0)\n\t"			\
					"lw $0, 4(%0)\n\t"			\
					"lw $0, 8(%0)\n\t"			\
					"lw $0, 12(%0)\n\t"			\
					"lw $0, 16(%0)\n\t"			\
					"lw $0, 20(%0)\n\t"			\
					"lw $0, 24(%0)\n\t"			\
					"lw $0, 28(%0)\n\t"			\
					"pref    0,128(%0)\n\t"		\
					"lw $0, 32(%0)\n\t"			\
					"lw $0, 36(%0)\n\t"			\
					"lw $0, 40(%0)\n\t"			\
					"lw $0, 44(%0)\n\t"			\
					"lw $0, 48(%0)\n\t"			\
					"lw $0, 52(%0)\n\t"			\
					"lw $0, 56(%0)\n\t"			\
					"lw $0, 60(%0)\n\t"			\
					"pref    0,160(%0)\n\t"		\
					"lw $0, 64(%0)\n\t"			\
					"lw $0, 68(%0)\n\t"			\
					"lw $0, 72(%0)\n\t"			\
					"lw $0, 76(%0)\n\t"			\
					"lw $0, 80(%0)\n\t"			\
					"lw $0, 84(%0)\n\t"			\
					"lw $0, 88(%0)\n\t"			\
					"lw $0, 92(%0)\n\t"			\
					"pref    0,192(%0)\n\t"		\
					"lw $0, 96(%0)\n\t"			\
					"lw $0, 100(%0)\n\t"		\
					"lw $0, 104(%0)\n\t"		\
					"lw $0, 108(%0)\n\t"		\
					"lw $0, 112(%0)\n\t"		\
					"lw $0, 116(%0)\n\t"		\
					"lw $0, 120(%0)\n\t"		\
					"lw $0, 124(%0)\n\t"		\
					:							\
					:"r"(x)						\
					);							\
			}while(0)
#endif


			unsigned int lsrc = (unsigned int)src;
			unsigned int ltar = (unsigned int)tar;
			while(lsrc < (unsigned int)src + size) {
#ifdef __mips__
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
				memread(lsrc);
				lsrc+=128;
#else
				lsrc+=1024;
#endif
			}
			while(ltar < (unsigned int)tar + size) {
#ifdef __mips__
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
				memread(ltar);
				ltar+=128;
#else
				ltar+=1024;
#endif
			}

		}
		return size * 2LL * loop;
	}
};

class DDRCopyTest :public PerfectTest
{
public:
	DDRCopyTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs):PerfectTest(size,loop,debugfs){
	}
	~DDRCopyTest(){}
private:
	/*
	 * Add memset to prevent tlb load loss when it write data.
	 */
	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		unsigned int *lsrc = (unsigned int *)src;
		unsigned int *ltar = (unsigned int *)tar;
		if(size & 3)
			return false;
		size = size / 4;
		for(unsigned int i = 0;i < size;i++){
			lsrc[i] = i;
			ltar[i] = size - i;
		}
		printf("Copy Test...\n");
		return true;
	}
	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop)
	{
		int l = loop;
		while(l--){
			memcpy(tar,src,size);
		}
		return size * 2LL * loop;
	}
	virtual bool postDDRTest(void * src,void *tar,unsigned int size)
	{
		return checkBuffer();
	}

};
class SimdDDRWriteTest: public DDRWriteTest
{
public:
	SimdDDRWriteTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs):DDRWriteTest(size,loop,debugfs){
	}
	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		printf("Simd Write Test...\n");
		memset(src,0xaa,size);
		memset(tar,0xaa,size);
		return true;
	}

	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop)
	{
		int l = loop;
		while(l--){
			simd_memset(src,0xaa,size);
			simd_memset(tar,0xaa,size);
		}
		return size * 2LL * loop;
	}

};
class SimdDDRCopyTest :public DDRCopyTest
{
public:
	SimdDDRCopyTest(unsigned int size,unsigned int loop,DebugFSOps *debugfs):DDRCopyTest(size,loop,debugfs){
	}

	virtual bool preDDRTest(void * src,void *tar,unsigned int size)
	{
		unsigned int *lsrc = (unsigned int *)src;
		unsigned int *ltar = (unsigned int *)tar;
		if(size & 3)
			return false;
		size = size / 4;
		for(unsigned int i = 0;i < size;i++){
			lsrc[i] = i;
			ltar[i] = size - i;
		}
		printf("Simd Copy Test...\n");
		return true;
	}
	virtual unsigned long long DDRTest(void * src,void *tar,unsigned int size,int loop)
	{
		int l = loop;
		while(l--){
			simd_memcpy(tar,src,size);
		}
		return size * 2LL * loop;
	}
};

static unsigned int getPackageSize(char *str)
{
	int len = strlen(str);
	char s[100];
	int unit = 1;
	int size = 512 * 1024;
	try{
	strcpy(s,str);
	if(len > 1){
		switch(s[len -1]){
		case 'K':
		case 'k':
			unit = 1024;
			s[len - 1] = 0;
			break;
		case 'M':
		case 'm':
			unit = 1024 * 1024;
			s[len - 1] = 0;
			break;
		case 'G':
		case 'g':
			unit = 1024 * 1024 * 1024;
			s[len - 1] = 0;
			break;
		}
	}
	size = atoi(s) * unit;
	}catch(...){
		printf("package argument error:%s\n",str);
	}
	return size;
}

int main(int argc, char *argv[])
{
	int test_flags, opt;
	int loops, packagesize;
	char *ddr_path = NULL;
	DebugFSOps *debugfs = NULL;

#define TEST_COPY		1
#define TEST_READ		2
#define TEST_WRITE		4
#define TEST_SIMD       8

	loops = 1;
	packagesize = 0;
	test_flags = 0;
	while ((opt = getopt(argc, argv, "rwcsl:p:d:")) != -1) {
		switch (opt) {
		case 'r':
			test_flags |= TEST_READ;
			break;
		case 'w':
			test_flags |= TEST_WRITE;
			break;
		case 'c':
			test_flags |= TEST_COPY;
			break;
		case 's':
			test_flags |= TEST_SIMD;
			break;
		case 'l':
			loops = atoi(optarg);
			break;
		case 'p':
			packagesize = getPackageSize(optarg);
			break;
		case 'd':
			ddr_path = optarg;
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-l loops] [-p packagesize] [-w] [-c] [-r] [-d debugfs]\n",
					argv[0]);
			fprintf(stderr, "-w only write test.\n");
			fprintf(stderr, "-r only read test.\n");
			fprintf(stderr, "-c copy test.\n");
			fprintf(stderr, "-p packagesize default 512K.\n");
			fprintf(stderr, "-l loops default 1.\n");
			fprintf(stderr, "-s simd optimized.\n");
			fprintf(stderr, "-d ddr bandwidth path in debugfs. example: -d '/mnt/ddr/'\n");
			return -1;
		}
	}
	// if (optind >= argc) {
	// 	fprintf(stderr, "Expected argument after options\n");
	// 	return -1;
	// }
	if(ddr_path)
		debugfs = new DebugFSOps(ddr_path);
	if(test_flags & TEST_COPY){
		new DDRCopyTest(packagesize,loops,debugfs);
		if(test_flags & TEST_SIMD)
			new SimdDDRCopyTest(packagesize,loops,debugfs);
	}
	if(test_flags & TEST_READ){
		new DDRReadTest(packagesize,loops,debugfs);
	}
	if(test_flags & TEST_WRITE){
		new DDRWriteTest(packagesize,loops,debugfs);
		if(test_flags & TEST_SIMD)
			new SimdDDRWriteTest(packagesize,loops,debugfs);
	}
	PerfectTest::TestAll();
    return 0;
}
