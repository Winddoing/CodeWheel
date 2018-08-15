/*============================================================================= 
 * #     FileName: rtspRequest.c
 * #         Desc: client connect to RTSP server
 * #       Author: licaibiao 
 * #   LastChange: 2017-04-19  
 * =============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 1024

void Handel_OPTION(char *inbuffer);
void Handel_DESCRIBE(char *inbuffer, int sockfd);
void Handel_SETUP(char *inbuffer, int sockfd);
void Handel_PLAY(char *inbuffer, int sockfd,char *sessionId);
void Handle_PAUSE(char *inbuffer,int sockfd, char *sessionId);
void Handel_Teardown(char *inbuffer, int sockfd,char *sessionId);


int   intsearch(char * buf, char * searchStr, int len);
int   strsearch(char * buf, char * searchStr, int len,char *dest);
int   init_udpsocket(int port, struct sockaddr_in *servaddr, char *mcast_addr);
void  release_udpsocket(int socket_fd, char *mcast_addr);
void  proc_udpsocket(int socket_fd, struct sockaddr_in servaddr,int flag);

char sendOPTRequstStr[] = "OPTIONS rtsp://192.168.0.6:8554/testStream RTSP/1.0\r\n";
char userAgentStr[] = "virtual client player\r\n";
char successfullyReplyStr[] = "RTSP/1.0 200 OK\r\n";
char sendDESRequstStr[] = "DESCRIBE rtsp://192.168.0.6:8554/testStream RTSP/1.0\r\n";
char sendSETUPRequstStr[] = "SETUP rtsp://192.168.0.6:8554/testStream/track1 RTSP/1.0\r\n";
char sendPLAYRequstStr[] = "PLAY rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char sendTeardownRequestStr[]="TEARDOWN rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char sendPauseRequestStr[]="PAUSE rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char CseqHeadStr[] = "Cseq: ";					
char UserHeadStr[] = "User-Agent: ";			
char ApplicationHeadStr[] = "Accept: ";			
char TransportHeadStr[] = "Transport: ";       
char SessionHeadStr[] = "Session: ";           
char RangeHeadStr[] = "Range: ";				
int cseqCount=0;

int main(int argc, char **argv)
{
	int sockfd,rtpsockfd,rtcpsockfd, len;
	struct sockaddr_in dest;      
	struct sockaddr_in RTPAddr;
	struct sockaddr_in RTCPAddr;
	char serverIP[]="192.168.0.6";
	char mcast_addr[] = "232.231.110.2"; 
	char buffer[MAXBUF + 1] = {0};
	int	serverPort=8554;
	int udpPort=0;

	cseqCount=0;

	/* creat socket for rtsp */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {      
		perror("Socket");
		exit(errno);
	}
	printf("socket created\n");

	/* init server add and port */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;     
	dest.sin_port   = htons(serverPort);    
	if (inet_pton(AF_INET, serverIP, &dest.sin_addr) < 0 ) {           
		perror(serverIP);
		exit(errno);
	}
	printf("address created\n");

	/* connect to server */
	if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
		perror("Connect error\n");
		exit(errno);
	}
	printf("server connected\n");

	/* MAXBUF use to receive message from stream server */
	bzero(buffer, MAXBUF + 1);

	/* send message to server */
	Handel_OPTION(buffer);
	len = send(sockfd, buffer, strlen(buffer), 0);
	if (len < 0)
		printf("OPTION Reply Msg:\n %s send error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
	else
		printf("OPTION Reply Msg:\n %s send successfully,total content is %d bytes!\n", buffer, len);
	bzero(buffer, MAXBUF + 1);

	/* receive message from server */
	len = recv(sockfd, buffer, MAXBUF, 0); /* OPTION Reply */
	if (len < 0){
		printf("OPTION Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));		
	}
	else{
		printf("OPTION Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);		
	}


	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr) -1) == 0){
		Handel_DESCRIBE(buffer, sockfd);
	}
	else{
		printf("the OPTION answer Msg is wrong!\n");
		return 0;
	}

	bzero(buffer, MAXBUF + 1);
	/* DESCRIBE Reply */
	len = recv(sockfd, buffer, MAXBUF, 0);
	if (len < 0){
		printf("DESCRIBE Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));		
	}
	else{
		printf("DESCRIBE Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);		
	}

	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) == 0){
		Handel_SETUP(buffer, sockfd);		
	}
	else{
		printf("the DESCRIBE answer Msg is wrong!\n");
		return 0;
	}

	bzero(buffer, MAXBUF + 1);    
	len = recv(sockfd, buffer, MAXBUF, 0);
	if (len < 0){
		printf("SETUP Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
	}
	else{
		printf("SETUP Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}

	char sessionIdTmp[30];
	bzero(sessionIdTmp,sizeof(sessionIdTmp));

	strsearch(buffer, "Session: ", 9, sessionIdTmp);
	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) == 0)
	{
		char searchStrTmp[13]="port=";
		udpPort=intsearch(buffer,searchStrTmp,5);
		if(udpPort==0){
			return 0;
		}
		printf("the udp port is %d\n",udpPort);

		bzero(&RTPAddr,sizeof(RTPAddr));
		bzero(&RTCPAddr,sizeof(RTCPAddr));

		rtpsockfd  = init_udpsocket(udpPort, &RTPAddr, mcast_addr);
		rtcpsockfd = init_udpsocket(udpPort+1, &RTCPAddr, mcast_addr);

		Handel_PLAY(buffer, sockfd,sessionIdTmp);
	}
	else
	{
		printf("the SETUP answer Msg is wrong!\n");
		return 0;
	}

	bzero(buffer, MAXBUF + 1);
	len = recv(sockfd, buffer, MAXBUF, 0);
	if (len < 0){	
		printf("PLAY Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));	
	}else{
		printf("PLAY Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}

	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) != 0){
		printf("the play answer Msg is wrong!\n");
		return 0;
	}

	bzero(buffer, MAXBUF + 1);
	printf("now,receiving RTP packets data......\n");
	proc_udpsocket(rtpsockfd, RTPAddr, 0);
	proc_udpsocket(rtcpsockfd, RTCPAddr, 1);

	Handle_PAUSE(buffer, sockfd, sessionIdTmp);

	len = recv(sockfd, buffer, MAXBUF, 0);
	if(len < 0){
		printf("PAUSE Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
	} 
	else{
		printf("PAUSE Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}


	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) != 0){
		printf("the pause answer Msg is wrong!\n");
		return 0;
	}

	bzero(buffer, MAXBUF + 1);
	Handel_Teardown(buffer, sockfd,sessionIdTmp);
	len = recv(sockfd, buffer, MAXBUF, 0);
	if (len < 0){
		printf("teardown Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));	
	}  
	else{
		printf("teardown Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}

	release_udpsocket(rtpsockfd, mcast_addr);
	release_udpsocket(rtcpsockfd, mcast_addr);
	return 0;
}

