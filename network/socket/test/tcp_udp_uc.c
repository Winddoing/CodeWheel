/*#############################################################
 *     File Name	: tcp_udp_uc.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月28日 星期五 18时49分14秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>

int port=8888;

int main(int argc,char *argv[]){
	int sockfd;
	int i=0;
	int z;
	char buf[80],str1[80];
	struct hostent *host;
	struct sockaddr_in adr_srvr;
	if(argc<2){
		fprintf(stderr,"please enter the server's hostname!\n");
		exit(1);
	}

	if((host=gethostbyname(argv[1]))==NULL){
		herror("gethostbyname error!");
		exit(1);
	}

	adr_srvr.sin_family=AF_INET;
	adr_srvr.sin_port=htons(port);
	adr_srvr.sin_addr=*((struct in_addr *)host->h_addr);
	bzero(&(adr_srvr.sin_zero),8);

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1){
		perror("socket error!");
		exit(1);
	}

	printf("msg to server:\n");
	fgets(buf, sizeof(buf) - 1, stdin);

	printf("send ....\n");
	z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_srvr,sizeof(adr_srvr));
	if(z<0){
		perror("sendto error");
		exit(1);
	}


	sprintf(buf,"stop\n");
	z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_srvr,sizeof(adr_srvr));
	if(z<0){
		perror("sendto error");
		exit(1);
	}

	close(sockfd);
	exit(0);
}
