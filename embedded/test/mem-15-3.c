#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int my_time()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp,sizeof(tmp),"%Y/%m/%d %X %A",localtime(&t) );
	puts(tmp);
	return 0;
}
int main(void)
{
	unsigned int size,seed,cacheline,i,j;

	int buf[]={32,64,96,128,256,512,4*1024,8*1024,16*1024,32*1024,256*1024,512*1024};
	char * space;
	struct timeval tv;
	struct timezone tz = {0,0};
	unsigned int addr,addrtemp;
	while(1){
		gettimeofday(&tv,&tz);
		seed = tv.tv_usec % 65536;
		srand(seed);
		size = 4*1024*1024 +((unsigned int)rand() % (9*1024*1024));
//		size = 1*1024*1024 +((unsigned int)rand() % (9*1024*1024));
		space = (char *)malloc(size);
		addrtemp = space;
		while(addrtemp<(space + size)){
			i = (unsigned int)rand()%(128*1024);
			*(char*)addrtemp = 0xa5;
			addrtemp+=i;
		}
		i = 500;
		printf("%d size = %d\n",i ,size);
		my_time();
		memset(space,0xa5,size);
		while(i--){
			cacheline = (unsigned int)rand() % (sizeof(buf)/(sizeof(buf[0])));
			addr = (unsigned int)rand()%size;
			//addr = ((unsigned int)space+addr)&~(0x1f);
			addr = ((unsigned int)space+addr)&~(4096-1);
			if( (addr>=(unsigned int )space) && (addr+buf[cacheline] < (unsigned int)space+size) ) {
				memcpy((void*)(addr+buf[cacheline]/2), (void*)addr , buf[cacheline]/2);
#if 1
				for(j = 0;j < (buf[cacheline]/2/4); j++){
					unsigned int s =*(unsigned int*)(addr + j * 4);
					unsigned int d = *(unsigned int *)(addr + buf[cacheline]/2 + j * 4);
					if(s != d){
						printf("s[%x]:%x d[%x]:%x\n",(addr + j *4),s,addr + buf[cacheline]/2 + j * 4,d);
					}
				}
#endif
			}else{
				i++;
			}
		}
		free(space);
	}
lil}
