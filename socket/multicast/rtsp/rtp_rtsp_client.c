/*=============================================================================  
 **     FileName: rtp_rtcp_client.c
 **         Desc: a client to recevie the rtp or rtcp data from live555 RTSP  
 **       Author: licaibiao  
 **   LastChange: 2017-04-20   
 **=============================================================================*/ 
#include<stdio.h>
#include<stdlib.h>  
#include<string.h>     
#include<unistd.h>      
#include<sys/types.h>    
#include<sys/socket.h>     
#include<arpa/inet.h>    
#include<netinet/in.h>    

#define MAXBUF 1024

int init_udpsocket(int port, struct sockaddr_in *servaddr, char *mcast_addr);
void release_udpsocket(int socket_fd, char *mcast_addr);
void proc_udpsocket(int socket_fd, struct sockaddr_in servaddr);

void main(void)
{
	int socket_fd;
	int rtp_port = 18888;
	int rtcp_port = 18888 + 1;
	char mcast_addr[] = "232.165.193.149";   
	struct sockaddr_in servaddr;

	socket_fd = init_udpsocket(rtcp_port, &servaddr, mcast_addr);
	proc_udpsocket(socket_fd, servaddr);
	release_udpsocket(socket_fd, mcast_addr);	
}

int init_udpsocket(int port, struct sockaddr_in *servaddr, char *mcast_addr)
{

	int err = -1;
	int socket_fd;                                      

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);    
	if (socket_fd < 0 )
	{
		perror("socket()");
		return -1;
	}  

	memset(servaddr, 0, sizeof(struct sockaddr_in));
	servaddr->sin_family	   = AF_INET;
	servaddr->sin_addr.s_addr  = htonl(INADDR_ANY);
	servaddr->sin_port		   = htons(port);

	err = bind(socket_fd,(struct sockaddr*)servaddr, sizeof(struct sockaddr_in)) ;
	if(err < 0)
	{
		perror("bind()");
		return -2;
	}

	/*set enable MULTICAST LOOP */                                       
	int loop = 1;
	err = setsockopt(socket_fd,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
	if(err < 0)
	{
		perror("setsockopt():IP_MULTICAST_LOOP");
		return -3;
	}

	/* Join multicast group */ 
	struct ip_mreq mreq; 
	mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr); 
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);      
	err = setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq));
	if (err < 0)
	{
		perror("setsockopt():IP_ADD_MEMBERSHIP");
		return -4;
	}
	return socket_fd;
}

void release_udpsocket(int socket_fd, char *mcast_addr)
{
	struct ip_mreq mreq; 
	mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr); 
	mreq.imr_interface.s_addr = htonl(INADDR_ANY); 
	setsockopt(socket_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(struct ip_mreq));
	close(socket_fd);
}

void proc_udpsocket(int socket_fd, struct sockaddr_in servaddr)
{
	int n;
	int i;
	int addr_len;
	char buff[MAXBUF + 1] = {0};

	memset(buff, 0, MAXBUF);    
	addr_len = sizeof(struct sockaddr_in);                                                        
	n = recvfrom(socket_fd, buff, MAXBUF, 0,(struct sockaddr*)&servaddr, &addr_len);
	if(n == -1)
	{
		perror("recvfrom()");
	}        
	printf("Recv %d byte message from server:\n", n);	
	for(i=0; i<n; i++)
	{
		printf("%d ",buff[i]);
	}
	printf("\n\n");	
}


