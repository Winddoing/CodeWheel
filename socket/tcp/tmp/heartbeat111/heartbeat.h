/*
 ** heartbeat.h
 **
 **  Created on: Jan 8, 2016
 **      Author: joaof
 **/

#ifndef SRC_NT_TELEM_PROJECT_MODULES_XTRAPOLIS_DATAMANAGER_HEARTBEAT_H_
#define SRC_NT_TELEM_PROJECT_MODULES_XTRAPOLIS_DATAMANAGER_HEARTBEAT_H_

#include <sys/socket.h>
#include <netinet/in.h>

// Structure with the heartbeat values from settings
struct heartBeat_values_s {
	char name[ 128];        // Heartbeat name
	int beatRate;           // Frequency the heartbeat is sent to shore

	char remoteIp[ 16];     // Server IP
	int  remotePort;        // Server port

	char udpPacket[1500];
	int sockfd;
	struct sockaddr_in sockaddr;
	unsigned int sockaddr_size;
	int counter;
};

struct heartBeat_values_s *heartbeat;
int nHeartbeat;

int heartbeat_init();
int heartbeat_processopts(struct secs *hbsec);
void *heartbeat_thread(void *dummy);

#endif /* SRC_NT_TELEM_PROJECT_MODULES_XTRAPOLIS_DATAMANAGER_HEARTBEAT_H_ */
