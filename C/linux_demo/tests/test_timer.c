#include <stdio.h>
#include <unistd.h>
#include <timer.h>

int main(int argc,char *argv[])
{
	char *systime=NULL;
	while(1)
	{	
	 	systime = time_get();
	 	printf("now time is:%s\n",systime);
	 	sleep(1);
	}
	return 0;
}
