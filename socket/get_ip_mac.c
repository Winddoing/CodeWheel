#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/if.h"
#include "arpa/inet.h"
#include "linux/sockios.h"

int main(int argc,char *argv[])
{
	struct sockaddr_in *addr;
	struct ifreq ifr;
	char*address;
	int sockfd;
	char *name = "eth0";

	if( strlen(name) >= IFNAMSIZ)
		printf("device name is error.\n"), exit(0);

	strcpy( ifr.ifr_name, name);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	//	get inet addr
	if( ioctl( sockfd, SIOCGIFADDR, &ifr) == -1)
		printf("ioctl error.\n"), exit(0);

	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	address = inet_ntoa(addr->sin_addr);
	printf("inet addr: %s\n",address);

	//get Mask
	if( ioctl( sockfd, SIOCGIFNETMASK, &ifr) == -1)
		printf("ioctl error.\n"), exit(0);
	addr = (struct sockaddr_in *)&ifr.ifr_addr;
	address = inet_ntoa(addr->sin_addr);
	printf("Mask: %s\n",address);

	//get HWaddr
	u_int8_t hd[6];
	if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
		printf("hwaddr error.\n"), exit(0);
	memcpy( hd, ifr.ifr_hwaddr.sa_data, sizeof(hd));
	printf("HWaddr: %02X:%02X:%02X:%02X:%02X:%02X\n", hd[0], hd[1], hd[2], hd[3], hd[4], hd[5]);

	exit(0);

}
