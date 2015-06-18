#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>

#define HOSTNAME "localhost"

int main(int argc, char *argv[])
{
	struct hostent *hostent;
	int i, j;

	hostent = gethostbyname(HOSTNAME);

	fprintf(stdout, "h_name = %s\n", hostent->h_name);
	fprintf(stdout, "h_addrtype = %s\n", 
					(AF_INET == hostent->h_addrtype ? "AF_INET" :
					 "AF_INET6"));
	fprintf(stdout, "h_length = %d\n", hostent->h_length);
	for(i = 0; hostent->h_aliases[i] != NULL; i++)
	{
		fprintf(stdout, "aliases[%d] = %s\n",
						i + 1, hostent->h_aliases[i]);
	}
	for(i = 0; hostent->h_addr_list[i] != NULL; i++)
	{
		fprintf(stdout, "h_addr_list[%d] = ",i + 1);
		for(j = 0; j < 4; j++)
		{
			fprintf(stdout, "%d ", hostent->h_addr_list[i][j]);
		}
		fprintf(stdout, "\n");
	}
	
	return EXIT_SUCCESS;
}

