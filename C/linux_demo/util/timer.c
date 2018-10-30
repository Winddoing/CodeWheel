#include<time.h>
#include<stdio.h>

char *time_get()
{
	time_t now;
	char *cur_time=NULL;
	if((now=time(NULL))==-1)
	{
		printf("get time fail");
		return NULL;
	}
	else
	{
	 	cur_time=ctime(&now);
	 	return cur_time;
	} 
	return NULL;
}