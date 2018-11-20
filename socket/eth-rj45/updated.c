#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

#include <linux/if_packet.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ether.h>

//TODO: rename ciaddr/yiaddr/chaddr
const uint8_t MAC_BCAST_ADDR[6] ALIGN2 = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

int gLocalFd = -1;

enum updated_state{

	UPDATED_WAIT_ETHERNET = 0,
	UPDATED_WAIT_DHCP_OFFER,
	UPDATED_GET_DHCP_OFFER,
};

enum updated_state gState = UPDATED_WAIT_ETHERNET;

void chkEthStatusHdlr(void *arg1, void* arg2);
void udhcp_recv_packets(int fd,void *eloop_ctx, void *sock_ctx);

uint16_t inet_cksum(uint16_t *addr, int nleft)
{
	/*
	 * Our algorithm is simple, using a 32 bit accumulator,
	 * we add sequential 16 bit words to it, and at the end, fold
	 * back all the carry bits from the top 16 bits into the lower
	 * 16 bits.
	 */
	unsigned sum = 0;
	while (nleft > 1) {
		sum += *addr++;
		nleft -= 2;
	}

	/* Mop up an odd byte, if necessary */
	if (nleft == 1) {
		if (BB_LITTLE_ENDIAN)
			sum += *(uint8_t*)addr;
		else
			sum += *(uint8_t*)addr << 8;
	}

	/* Add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */

	return (uint16_t)~sum;
}

int udhcp_end_option(uint8_t *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] != DHCP_PADDING)
			i += optionptr[i + OPT_LEN] + OPT_DATA-1;
		i++;
	}
	return i;
}


/* Returns -1 on errors that are fatal for the socket, -2 for those that aren't */
/* NOINLINE: limit stack usage in caller */
int udhcp_recv_raw_packet(struct dhcp_packet *dhcp_pkt, int fd)
{
	int bytes;
	struct ip_udp_dhcp_packet packet;
	uint16_t check;
	unsigned char cmsgbuf[CMSG_LEN(sizeof(struct tpacket_auxdata))];
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;

	/* used to use just safe_read(fd, &packet, sizeof(packet))
	 * but we need to check for TP_STATUS_CSUMNOTREADY :(
	 */
	iov.iov_base = &packet;
	iov.iov_len = sizeof(packet);
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	for (;;) {
		bytes = recvmsg(fd, &msg, 0);
		if (bytes < 0) {
			printf("udhcp_recv_raw_packet recvmsg failed errno %d\n",errno);
			if (errno == EINTR)
				continue;
			printf("packet read error, ignoring\n");
			/* NB: possible down interface, etc. Caller should pause. */
			return bytes; /* returns -1 */
		}
		break;
	}

	if (bytes < (int) (sizeof(packet.ip) + sizeof(packet.udp))) {
		printf("packet is too short, ignoring\n");
		return -2;
	}

	if (bytes < ntohs(packet.ip.tot_len)) {
		/* packet is bigger than sizeof(packet), we did partial read */
		printf("oversized packet, ignoring\n");
		return -2;
	}

	/* ignore any extra garbage bytes */
	bytes = ntohs(packet.ip.tot_len);

	/* make sure its the right packet for us, and that it passes sanity checks */
	if (packet.ip.protocol != IPPROTO_UDP
			|| packet.ip.version != IPVERSION
			|| packet.ip.ihl != (sizeof(packet.ip) >> 2)
			|| packet.udp.dest != htons(CLIENT_PORT)
			/* || bytes > (int) sizeof(packet) - can't happen */
			|| ntohs(packet.udp.len) != (uint16_t)(bytes - sizeof(packet.ip))
	   ) {
		printf("unrelated/bogus packet, ignoring\n");
		return -2;
	}

	/* verify IP checksum */
	check = packet.ip.check;
	packet.ip.check = 0;
	if (check != inet_cksum((uint16_t *)&packet.ip, sizeof(packet.ip))) {
		printf("bad IP header checksum, ignoring\n");
		return -2;
	}

	for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
		if (cmsg->cmsg_level == SOL_PACKET
				&& cmsg->cmsg_type == PACKET_AUXDATA
		   ) {
			/* some VMs don't checksum UDP and TCP data
			 * they send to the same physical machine,
			 * here we detect this case:
			 */
			struct tpacket_auxdata *aux = (void *)CMSG_DATA(cmsg);
			if (aux->tp_status & TP_STATUS_CSUMNOTREADY)
				goto skip_udp_sum_check;
		}
	}

	/* verify UDP checksum. IP header has to be modified for this */
	memset(&packet.ip, 0, offsetof(struct iphdr, protocol));
	/* ip.xx fields which are not memset: protocol, check, saddr, daddr */
	packet.ip.tot_len = packet.udp.len; /* yes, this is needed */
	check = packet.udp.check;
	packet.udp.check = 0;
	if (check && check != inet_cksum((uint16_t *)&packet, bytes)) {
		printf("packet with bad UDP checksum received, ignoring\n");
		return -2;
	}
skip_udp_sum_check:

	if (packet.data.cookie != htonl(DHCP_MAGIC)) {
		printf("packet with bad magic, ignoring\n");
		return -2;
	}

	printf("received %s", "a packet\n");


	bytes -= sizeof(packet.ip) + sizeof(packet.udp);
	memcpy(dhcp_pkt, &packet.data, bytes);
	return bytes;
}

uint8_t* udhcp_get_option(struct dhcp_packet *packet, int code)
{
	uint8_t *optionptr;
	int len;
	int rem;
	int overload = 0;
	enum {
		FILE_FIELD101  = FILE_FIELD  * 0x101,
		SNAME_FIELD101 = SNAME_FIELD * 0x101,
	};

	/* option bytes: [code][len][data1][data2]..[dataLEN] */
	optionptr = packet->options;
	rem = sizeof(packet->options);
	while (1) {
		if (rem <= 0) {
			printf("bad packet, malformed option field\n");
			return NULL;
		}
		if (optionptr[OPT_CODE] == DHCP_PADDING) {
			rem--;
			optionptr++;
			continue;
		}
		if (optionptr[OPT_CODE] == DHCP_END) {
			if ((overload & FILE_FIELD101) == FILE_FIELD) {
				/* can use packet->file, and didn't look at it yet */
				overload |= FILE_FIELD101; /* "we looked at it" */
				optionptr = packet->file;
				rem = sizeof(packet->file);
				continue;
			}
			if ((overload & SNAME_FIELD101) == SNAME_FIELD) {
				/* can use packet->sname, and didn't look at it yet */
				overload |= SNAME_FIELD101; /* "we looked at it" */
				optionptr = packet->sname;
				rem = sizeof(packet->sname);
				continue;
			}
			break;
		}
		len = 2 + optionptr[OPT_LEN];
		rem -= len;
		if (rem < 0)
			continue; /* complain and return NULL */

		if (optionptr[OPT_CODE] == code) {
			printf("option %d found\n", *optionptr);
			return optionptr + OPT_DATA;
		}

		if (optionptr[OPT_CODE] == DHCP_OPTION_OVERLOAD) {
			overload |= optionptr[OPT_DATA];
			/* fall through */
		}
		optionptr += len;
	}

	/* log3 because udhcpc uses it a lot - very noisy */
	printf("option 0x%02x not found\n", code);
	return NULL;
}


int get_dhcp_packet_type(struct dhcp_packet *p)
{
	uint8_t *op;

	/* it must be either a BOOTREQUEST or a BOOTREPLY */
	if (p->op != BOOTREQUEST && p->op != BOOTREPLY)
		return -1;
	/* get message type option */
	op = udhcp_get_option(p, DHCP_MESSAGE_TYPE);
	if (op != NULL)
		return op[0];
	return -1;
}


void wait_dhcp_offer_timeout(void *arg1, void* arg2)
{
	int fd = *(int*)(arg2);
	printf("wait_dhcp_offer_timeout\n");
	veloop_unregister_read_sock( fd);
	close(fd);
	veloop_register_timeout(5,0,chkEthStatusHdlr,NULL,NULL);
}



int wait_dhcp_offer(int ifindex)
{
	int fd;
	struct sockaddr_ll sock;
	int optval = 1;
	int ret;
	int type = -1;


	printf("opening raw socket on ifindex %d\n", ifindex); //log2?

	fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));


	sock.sll_family = AF_PACKET;
	sock.sll_protocol = htons(ETH_P_IP);
	sock.sll_ifindex = ifindex;
	bind(fd, (struct sockaddr *) &sock, sizeof(sock));


	if (setsockopt(fd, SOL_PACKET, PACKET_AUXDATA, &optval, sizeof(int)) != 0) {
		if (errno != ENOPROTOOPT)
			printf("can't set PACKET_AUXDATA on raw socket");
	}
	gState = UPDATED_WAIT_DHCP_OFFER;
	gLocalFd = fd;
	veloop_register_sock(fd,udhcp_recv_packets,NULL,NULL);
	veloop_register_timeout(5,0,wait_dhcp_offer_timeout ,NULL,&gLocalFd);


}

/* Get an option with bounds checking (warning, result is not aligned) */



/* Construct a ip/udp header for a packet, send packet */
int udhcp_send_raw_packet(struct dhcp_packet *dhcp_pkt,
		uint32_t source_nip, int source_port,
		uint32_t dest_nip, int dest_port, const uint8_t *dest_arp,
		int ifindex)
{
	struct sockaddr_ll dest_sll;
	struct ip_udp_dhcp_packet packet;
	unsigned padding;
	int fd;
	int result = -1;
	const char *msg;

	fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	if (fd < 0) {
		msg = "socket(%s)";
		goto ret_msg;
	}

	memset(&dest_sll, 0, sizeof(dest_sll));
	memset(&packet, 0, offsetof(struct ip_udp_dhcp_packet, data));
	packet.data = *dhcp_pkt; /* struct copy */

	dest_sll.sll_family = AF_PACKET;
	dest_sll.sll_protocol = htons(ETH_P_IP);
	dest_sll.sll_ifindex = ifindex;
	dest_sll.sll_halen = 6;
	memcpy(dest_sll.sll_addr, dest_arp, 6);

	if (bind(fd, (struct sockaddr *)&dest_sll, sizeof(dest_sll)) < 0) {
		msg = "bind(%s)";
		goto ret_close;
	}

	/* We were sending full-sized DHCP packets (zero padded),
	 * but some badly configured servers were seen dropping them.
	 * Apparently they drop all DHCP packets >576 *ethernet* octets big,
	 * whereas they may only drop packets >576 *IP* octets big
	 * (which for typical Ethernet II means 590 octets: 6+6+2 + 576).
	 *
	 * In order to work with those buggy servers,
	 * we truncate packets after end option byte.
	 *
	 * However, RFC 1542 says "The IP Total Length and UDP Length
	 * must be large enough to contain the minimal BOOTP header of 300 octets".
	 * Thus, we retain enough padding to not go below 300 BOOTP bytes.
	 * Some devices have filters which drop DHCP packets shorter than that.
	 */
	padding = DHCP_OPTIONS_BUFSIZE - 1 - udhcp_end_option(packet.data.options);
	if (padding > DHCP_SIZE - 300)
		padding = DHCP_SIZE - 300;

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source_nip;
	packet.ip.daddr = dest_nip;
	packet.udp.source = htons(source_port);
	packet.udp.dest = htons(dest_port);
	/* size, excluding IP header: */
	packet.udp.len = htons(UDP_DHCP_SIZE - padding);
	/* for UDP checksumming, ip.len is set to UDP packet len */
	packet.ip.tot_len = packet.udp.len;
	packet.udp.check = inet_cksum((uint16_t *)&packet,
			IP_UDP_DHCP_SIZE - padding);
	/* but for sending, it is set to IP packet len */
	packet.ip.tot_len = htons(IP_UDP_DHCP_SIZE - padding);
	packet.ip.ihl = sizeof(packet.ip) >> 2;
	packet.ip.version = IPVERSION;
	packet.ip.ttl = IPDEFTTL;
	packet.ip.check = inet_cksum((uint16_t *)&packet.ip, sizeof(packet.ip));


	result = sendto(fd, &packet, IP_UDP_DHCP_SIZE - padding, /*flags:*/ 0,
			(struct sockaddr *) &dest_sll, sizeof(dest_sll));
	if(result<0)
		printf("sendto errno %d\n",errno);
	msg = "sendto";
ret_close:
	close(fd);
	if (result < 0) {
ret_msg:
		printf("%s PACKET\n",msg);
	}
	return result;
}


int send_dhcp_discover(const char* ifname, uint8_t *macAddr)
{
	struct dhcp_packet packet;
	int ifindex  = 0;


	if(ifname == NULL)
		return -1;


	memset(&packet, 0, sizeof(packet));
	packet.op = 1; /* if client to a server */
	packet.htype = 1; /* ethernet */
	packet.hlen = 6;
	packet.xid = rand();
	packet.cookie = htonl(DHCP_MAGIC);


	memcpy(packet.chaddr,macAddr,6);//our mac address
	packet.options[0] = 0x35;/*dhcp message type*/
	packet.options[1] = 1;/*length 1*/
	packet.options[2] = 1;/*dhcp discover*/

	packet.options[3] = 0x3d;/*client identifier*/
	packet.options[4] = 7;/*length*/
	packet.options[5] = 1;/*hardware type, 0x01 for ethernet*/
	memcpy(packet.options+6,macAddr,6);
	packet.options[12] = 0xFF;/*options end*/

	ifindex = if_nametoindex(UPDATED_IFACE_NAME);

	printf("sending dhcp disconver ifindex %d\n",ifindex);


	return udhcp_send_raw_packet(&packet,INADDR_ANY,68,INADDR_BROADCAST,67,MAC_BCAST_ADDR,ifindex);
}


/* 检查网卡与网线的连接状态,是否连接 */
int checkRJ45(char *ifname)
{
	int fd = -1;
	struct ifreq ifreq;
	struct ethtool_value edata;

	if(NULL == ifname)
	{
		printf("need valid ifname\n");
		return -1;
	}

	fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(fd < 0)
	{
		printf("%s get socket fd failed with error %d\n",__func__,errno);
		return -1;
	}

	memset(&ifreq, 0, sizeof(struct ifreq));
	strcpy(ifreq.ifr_name, ifname);

	edata.cmd = ETHTOOL_GLINK;
	ifreq.ifr_data = (void*) &edata;

	if (ioctl(fd,SIOCETHTOOL, &ifreq) < 0) {
		printf("%s SIOCETHTOOL failed with error %d\n",__func__,errno );
		return -1;
	}

	return edata.data ? 1 : 0;
}

int getEthMacAddr(const char *ifname,uint8_t *macAddr)
{
	struct ifreq req;
	int sock = -1;

	if(NULL == ifname)
		goto fail;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		goto fail;

	memset(&req,0,sizeof(struct ifreq));
	strcpy(req.ifr_name, ifname);

	if (ioctl(sock, SIOCGIFHWADDR, &req) < 0) {
		goto fail;
	}
	memcpy(macAddr, req.ifr_addr.sa_data, 6);
	close(sock);
	return 0;

fail:
	if (sock >= 0)
		close(sock);
	return -1;

}

/* ifconfig eth0 up/down */
int setIfaceFlags(const char *ifname,uint32_t flags)
{
	struct ifreq ifr;
	int s;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("socket");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);


	if (ioctl(s, SIOCGIFFLAGS, &ifr) < 0) {
		perror("ioctl[SIOCGIFFLAGS]");
		close(s);
		return -1;
	}
	ifr.ifr_flags |= flags;

	if (ioctl(s, SIOCSIFFLAGS, &ifr) < 0) {
		perror("ioctl[SIOCSIFFLAGS]");
		close(s);
		return -1;
	}
	close(s);
	return 0;
}

void udhcp_recv_packets(int fd,void *eloop_ctx, void *sock_ctx)
{
	struct dhcp_packet dhcp_pkt;
	int ret;
	int type = -1;


	ret = udhcp_recv_raw_packet(&dhcp_pkt,fd);

	if(ret<0)
		printf("recv raw packets failed\n");
	else{
		type = get_dhcp_packet_type(&dhcp_pkt);

		printf("get dhcp packet type %d\n",type);
	}
	printf("cancel wait_dhcp_offer_timeout\n");
	veloop_unregister_read_sock(fd);
	veloop_cancel_timeout(wait_dhcp_offer_timeout,NULL,&gLocalFd);
	close(fd);
	if(type == DHCPOFFER)
	{
		printf("get dhcp offer!!!\n");
		system("/etc/scripts/updated_get_ip.sh");
	}
	else
	{
		printf("get dhcp offer failed type %d\n",type);
		veloop_register_timeout(5,0,chkEthStatusHdlr,NULL,NULL);
	}

}

void chkEthStatusHdlr(void *arg1, void* arg2)
{
	int ret = 0;
	int fd;
	uint8_t macAddr[6]={0};

	printf("checking ethernet status ...\n");
	ret = checkRJ45(UPDATED_IFACE_NAME);
	if(ret < 0)
	{
		printf("ethernet not present\n");
		veloop_register_timeout(5,0,chkEthStatusHdlr,NULL,NULL);
		return ;
	}
	else if(ret == 0)
	{
		printf("ethernet present, status down\n");
		setIfaceFlags(UPDATED_IFACE_NAME,IFF_UP);
	}
	else if(ret == 1)
	{
		printf("ethernet present, status up\n");
	}

	system("/etc/scripts/updated_get_ip.sh");

}

void redirect_tty_to_null(void)
{
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);
	return ;
}

int main(int argc, char **argv)
{
	int sockFd = 0;

	redirect_tty_to_null();

	veloop_init();
	veloop_register_timeout(5,0,chkEthStatusHdlr,NULL,NULL);
	veloop_run();

	return 0;
}
