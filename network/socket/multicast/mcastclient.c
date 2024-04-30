#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <time.h>  
#include <string.h>  
#include <stdio.h>  
#include <unistd.h>  

//#define HELLO_PORT  12345  
//#define HELLO_GROUP "225.0.0.37"  
#define MSGBUFSIZE 32*1024  

int main(int argc, char *argv[])  
{  
	if (argc != 4)
	{
		printf ("%s  mulitAddr  port  bindIP\n", argv[0]);
		exit(129);
	}
	char* multiAddr = argv[1];
	int port = atoi(argv[2]);
	char* bindIP = argv[3];

	printf ("%s:%d\n", multiAddr, port);
	printf ("bindIP = %s\n", bindIP);

	struct sockaddr_in addr;  
	int fd, nbytes,addrlen;  
	struct ip_mreq mreq;  
	char msgbuf[MSGBUFSIZE];  

	u_int yes=1; /*** MODIFICATION TO ORIGINAL */  

	/* create what looks like an ordinary UDP socket */  
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)   
	{  
		perror("socket");  
		exit(1);  
	}  


	/**** MODIFICATION TO ORIGINAL */  
	/* allow multiple sockets to use the same PORT number */  
	if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)   
	{  
		perror("Reusing ADDR failed");  
		exit(1);  
	}  
	/*** END OF MODIFICATION TO ORIGINAL */  

	/* set up destination address */  
	memset(&addr,0,sizeof(addr));  
	addr.sin_family=AF_INET;  
	addr.sin_addr.s_addr=INADDR_ANY; /* N.B.: differs from sender */  
	addr.sin_port=htons(port);  

	/* bind to receive address */  
	if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0)  
	{  
		perror("bind");  
		exit(1);  
	}  

	/* use setsockopt() to request that the kernel join a multicast group */  
	mreq.imr_multiaddr.s_addr=inet_addr(multiAddr);  
/*	mreq.imr_interface.s_addr=inet_addr(bindIP);//htonl(INADDR_ANY);  */
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);  
	if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)   
	{  
		perror("setsockopt");  
		exit(1);  
	}  

	/* now just enter a read-print loop */  
	while (1)   
	{  
		addrlen=sizeof(addr);  
		if ((nbytes=recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr,(socklen_t*)&addrlen)) < 0)   
		{  
			perror("recvfrom");  
			exit(1);  
		}  
		printf("r: %s\n", msgbuf);
		printf ("write data %d bytes\n", nbytes);
	}  

	return 0;  
}  
