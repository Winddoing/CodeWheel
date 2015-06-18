#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int retPid;

	retPid = fork();

	switch(retPid)
	{
		case -1: 
			return EXIT_FAILURE;
		case 0:
			sleep(6);
			fprintf(stdout, "xxxxxxxxxxxxxx");
			kill(retPid, SIGKILL);
			//setuid(0);
			//chdir("/");
			//umask(0);
			for(;;)
			{
				sleep(1);
				fprintf(stdout,"I'm child."
					"PID = %d, PPID = %d",
					getpid(), getppid());
			}
			break;
		default:
			for(;;)
			{
				sleep(1);
				fprintf(stdout,"I'm parent."
					"PID = %d, CPID = %d",
					getpid(), retPid);
			}
	}
		
	return EXIT_SUCCESS;
}
