#include <stdio.h>
#include <time.h>
#include "udp.h"
#include "led.h"
#include "protocol.h"

int main(int argc, char *argv[])
{
	int ret, len;
	char buffer[1024];
	char led_state[3];
	int recv_num;
	int recv_cmd;
	ret = udp_init(DSET_IP_ADDRESS, DEST_PORT);
	if(ret <= 0){
		printf("UDP socket init failed\n");
		return -1;
	}
	led_ctrl('r','0');
	led_ctrl('g','0');
	led_ctrl('b','0');

	int last_comm_time = 0;
	while(1){
		//usleep(100000);
		if(time(0) - last_comm_time > HEARTBEAT_INTV){
			len = str_login(buffer);
			udp_send(buffer, len);
			usleep(200000);
		}
		recv_num = udp_recv(buffer, 1024);
		if(recv_num > 0){
			if((ret = check_buf(buffer, recv_num)) < 0){
				printf("recieve data,but error:%d\n", ret);
				continue;
			}
			printf("recieve right data\n");
			last_comm_time = time(0);
			switch(buffer[CMD_POS+1]){
				case CMD_ACK_LOGIN :
					printf("CMD_ACK_LOGIN \n");
					break;
				case CMD_SET_STATUS	:
					if(buffer[DATA_POS+16] == 1){
						led_ctrl('r','0'+buffer[DATA_POS+17]);
					}
					if(buffer[DATA_POS+16] == 2){
						led_ctrl('g','0'+buffer[DATA_POS+17]);
					}
					if(buffer[DATA_POS+16] == 3){
						led_ctrl('b','0'+buffer[DATA_POS+17]);
					}
				case CMD_GET_STATUS	:
					len = str_report(buffer);
					udp_send(buffer, len);
					printf("CMD_SET_or_GET_STATUS	\n");
					break;
			}
		}
	}
}
