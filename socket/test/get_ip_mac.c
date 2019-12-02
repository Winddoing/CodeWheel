#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h> //struct ifreq
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	struct sockaddr_in *addr;
	struct ifreq ifr;
	char*address;
	int fd;
	char *name = "enp2s0";

	if (argc > 1)
		name = argv[1];

	if (strlen(name) >= IFNAMSIZ) {
		printf("device name is error.\n");
		return -1;
	}

	fd = socket(AF_INET,SOCK_DGRAM,0);
	if (-1 == fd) {
		perror("Creat socket:");
		return -1;
	}

	bzero(&ifr, sizeof(ifr));
	strcpy(ifr.ifr_name, name);

	//	get inet addr
	if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
		perror("Get inet addr: ");
		printf("Usge: %s eth1\n", argv[0]);
		goto out;
	}

	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	address = inet_ntoa(addr->sin_addr);
	printf("inet addr: %s\n",address);

	//get Mask
	if (ioctl( fd, SIOCGIFNETMASK, &ifr) == -1) {
		perror("Get mask:");
		goto out;
	}
	addr = (struct sockaddr_in *)&ifr.ifr_addr;
	address = inet_ntoa(addr->sin_addr);
	printf("Mask: %s\n",address);

	//get HWaddr
	u_int8_t hd[6];
	if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
		perror("Get HWaddr:");
		goto out;
	}
	memcpy( hd, ifr.ifr_hwaddr.sa_data, sizeof(hd));
	printf("HWaddr: %02X:%02X:%02X:%02X:%02X:%02X\n", hd[0], hd[1], hd[2], hd[3], hd[4], hd[5]);

	//get broadcast addr
	if (ioctl(fd, SIOCGIFBRDADDR, &ifr) == -1) {
		perror("Get broadcast addr:");
		goto out;
	}
	printf("broadcast addr: %s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));

out:
	close(fd);
	return 0;
}
