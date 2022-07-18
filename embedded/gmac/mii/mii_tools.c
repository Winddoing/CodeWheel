/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : mii_tools.c
 *  Author       : wqshao
 *  Created Time : 2022-07-06 16:00:36
 *  Description  : 读写phy中的寄存器
 *		aarch64-none-linux-gnu-gcc mii_tools.c -o mii_tools
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/mii.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <netinet/in.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int sockfd;
	struct mii_ioctl_data *mii = NULL;
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
	sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	ioctl(sockfd, SIOCGMIIPHY, &ifr);

	mii = (struct mii_ioctl_data*)&ifr.ifr_data;

	if(argc == 4) {
		strncpy(ifr.ifr_name, argv[1], IFNAMSIZ - 1);
		sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
		ioctl(sockfd, SIOCGMIIPHY, &ifr);

		mii->phy_id  = (uint16_t)strtoul(argv[2], NULL, 0);
		mii->reg_num = (uint16_t)strtoul(argv[3], NULL, 0);

		ioctl(sockfd, SIOCGMIIREG, &ifr);

		printf("read --- value : 0x%x\n", mii->val_out);
	} else if(argc == 5) {
		strncpy(ifr.ifr_name, argv[1], IFNAMSIZ - 1);
		sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
		ioctl(sockfd, SIOCGMIIPHY, &ifr);

		mii->phy_id  = (uint16_t)strtoul(argv[2], NULL, 0);
		mii->reg_num = (uint16_t)strtoul(argv[3], NULL, 0);
		mii->val_in  = (uint16_t)strtoul(argv[4], NULL, 0);

		ioctl(sockfd, SIOCSMIIREG, &ifr);
	} else {
		printf("mii_tools ethX phyId addr value\n");
	}

	close(sockfd);



	return 0;
}
