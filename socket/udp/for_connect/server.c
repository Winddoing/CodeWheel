/*#############################################################
 *     File Name	: server.c
 *     Author		: winddoing
 *     Created Time	: 2019年10月10日 星期四 00时07分12秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>


int main(int argc, const char *argv[])
{
    int sockFd = -1;
    in_addr_t addr;
    struct sockaddr_in localAddr;
    struct sockaddr_in remoteAddr;

    //step 1.creat socket file
    sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == sockFd) {
        printf("Create socket fail..\n");
        return -1;
    }

    //step 2.set localAddr struct
    bzero(&localAddr, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = inet_addr("192.168.42.153");
    localAddr.sin_port = htons(1120);

    //step 3.bind socket info
    if(-1 == bind(sockFd, (struct sockaddr *)&localAddr, sizeof(localAddr))) {
        close(sockFd);
        sockFd = -1;
        printf("local socket info bind fail\n");
        return -1;
    }

    //step 4.set remoteAddr struct
    bzero(&remoteAddr, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_addr.s_addr = inet_addr("192.168.42.153");
    remoteAddr.sin_port = htons(1121);

    //step 5.connect socket info
    if(-1 == connect(sockFd, (struct sockaddr *)&localAddr, sizeof(localAddr))) {
        close(sockFd);
        sockFd = -1;
        printf("remote socket info connect fail\n");
        return -1;
    }

    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    int ret = 0;
    fd_set fds; 
    struct timeval tv; 

    tv.tv_sec = 100;   
    tv.tv_usec = 0;  

    FD_ZERO(&fds);
    FD_SET(sockFd, &fds);
    ret = select(sockFd + 1, &fds, NULL, NULL, &tv); 
    if(-1 == ret) {
        printf("select error\n");
        return -1; 
    }
    if(0 == ret) {
        printf("select timeout\n");
        return -1;
    }


    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    char buf[20];

    ret = read(sockFd, buf, sizeof(buf));
    printf("===> func: %s, line: %d, buf=%s\n", __func__, __LINE__, buf);


    close(sockFd);

    return 0;
}
