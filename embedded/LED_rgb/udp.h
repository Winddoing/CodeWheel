#define UDP_SEND_BUFFER_SIZE 1024
#define UDP_RECV_BUFFER_SIZE 1024
#define DSET_IP_ADDRESS  "106.37.171.197"
#define DEST_PORT 6000
typedef enum{
	udp_unconnect,
	udp_connect,
}udp_state_t;

int udp_init(char *ip, int port);
int udp_send(char *send_buf, int len);
int udp_recv(char *recv_buf, int max_len);
