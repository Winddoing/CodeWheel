/*
 * heartbeat.c
 *
 *  Created on: Jan 8, 2016
 *      Author: joaof
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <err.h>
#include <time.h>
#include <pthread.h>

#include "../logger/logger.h"
#include "../global/global.h"
#include "../globalvars.h"
#include "../netstat/netstat.h"
#include "../filereader/filereader.h"
#include "heartbeat.h"

pthread_t hbpthread_tid;

extern char swversion[ 5];

int heartbeat_processopts(struct secs *hbsec){

	int i, n, varsCount=0;
	char *tok;

	if (strcasecmp(hbsec->seccao, "HEARTBEAT") == 0) {

		nHeartbeat = 0;

		for(i = 0; i < hbsec->con_entries; i++) {
			if( strcmp(hbsec->sec_entries[ i].esq,"heartbeat") == 0) {
				//
				varsCount = 1;

				// Count number of atributes the "heartbeat" key has
				tok = strpbrk( hbsec->sec_entries[ i].dir,",");

				while( tok != NULL) {
					varsCount++;
					tok = strpbrk (tok+1,",");
				}

				// Sanity check - At the moment the heartbeat needs to have 4 arguments, it may change
				if( varsCount != 4)
					return -1;;

				// Add new heartbeat
				nHeartbeat++;
				heartbeat = (struct heartBeat_values_s *) realloc( heartbeat, nHeartbeat * sizeof( struct heartBeat_values_s));

				// This heartbeat index
				n = nHeartbeat - 1;

				// Init heartbeat parameters
				// Heartbeat - name
				tok = strtok( hbsec->sec_entries[i].dir,",");
				strcpy( heartbeat[ n].name, tok);

				// Heartbeat - beatrate
				tok = strtok (NULL,",");
				heartbeat[ n].beatRate = atoi( tok);

				// Heartbeat - remote ip
				tok = strtok (NULL,",");
				strcpy(heartbeat[ n].remoteIp, tok);

				// Heartbeat - remote port
				tok = strtok (NULL,",");
				heartbeat[ n].remotePort = atoi( tok);
			}
		}
	} else {
		printf("error: SETTINGS Section #HEARTBEAT not defined\n");
		return 1;
	}
	return 0;
}

static int init_udp_socket(int reuse_f) {

	int sockfd;

	/*Open an UDP socket*/
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		logger_write(ntcommlog, "<warn><heartbeat>Error creating socket (%d %s)", errno, strerror(errno));
		return -1;
	}

	/*Make the socket able to reuse an open IP/port with the same protocol*/
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse_f, sizeof(int));

	return sockfd;
}

static struct sockaddr_in init_sock_addr( const char *remoteIp, const int remotePort) {

	struct sockaddr_in sockaddr;

	/*Build the remote Internet address*/
	bzero( (char*)&sockaddr, sizeof( sockaddr));
	sockaddr.sin_family      =  AF_INET;
	sockaddr.sin_addr.s_addr =  inet_addr(remoteIp);
	sockaddr.sin_port        =  htons(remotePort);

	return sockaddr;
}

// Create the frame with the connection status
//
static int create_frame_netStatus(char *outputFrame, int flag) {
	//
	int ret;
	char frame[ 500];
	char subframe[200];

	// Timestamp variables
	int D,M,Y,h,m,s;

	if(flag == 1) {
		// Get time
		GetTime(&D, &M, &Y, &h, &m, &s);

		// Fill the header
		memset(frame, 0, sizeof(frame));
		sprintf(frame, "NETSTATUS_N\t%s\t%02d%02d%02d-%02d%02d%02d\t%s\t", ntcomm_values.unit, Y, M, D, h, m, s, "ntcomm_v");

		// Fill the payload
		memset(subframe, 0, sizeof(subframe));
		sprintf(subframe, "status\tip\tinterface\trxBytes\trxPackets\trxErrors\ttxBytes\ttxPackets\ttxErrors");

		// Concatenate
		strcat(frame, subframe);

		// count number of bytes
		ret = sprintf(outputFrame, "%s", frame);

		return ret;
	}
	else if(flag == 2){
		// Get time
		GetTime(&D, &M, &Y, &h, &m, &s);

		// Fill the header
		memset(frame, 0, sizeof(frame));
		sprintf(frame, "NETSTATUS\t%s\t%02d%02d%02d-%02d%02d%02d\t%s\t", ntcomm_values.unit, Y, M, D, h, m, s, swversion);

		// Fill the payload
		memset(subframe, 0, sizeof(subframe));
		sprintf(subframe, "%c\t%s\t%s\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu", myConnection.status,
				myConnection.myIp,
				currentValue.interfaceName,
				currentValue.rxBytes,
				currentValue.rxPackets,
				currentValue.rxErrors,
				currentValue.txBytes,
				currentValue.txPackets,
				currentValue.txErrors);

		// Concatenate
		strcat(frame, subframe);

		// Count number of bytes
		ret = sprintf(outputFrame, "%s", frame);

		return ret;
	}
	else
		return -1;
}

/* ***************************************************************************************
 *  [description]       Starts all the threads to manage heartbeats
 *
 *  [return] void
 * ***************************************************************************************/
int heartbeat_init() {
	//
	int err;
	pthread_attr_t attr;

	// Start logging
	logger_write(ntcommlog, "<info><heartbeat> Module Init");

	pthread_attr_init( &attr);
	//pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED);

	// Thread creation
	if((err = pthread_create(&hbpthread_tid, &attr, heartbeat_thread, NULL)) != 0 ) {
		logger_write(ntcommlog, "<warn><heartbeat>Error Creating Pthread: [%s]", strerror(err));
		return -1;
	}
	sleep(1);

	printf("\nExit hearbeat init\n");

	return 0;
}

/* ***************************************************************************************
 *  [description]       Thread for managing the heartbeat
 *
 *
 *  [return] void
 * ***************************************************************************************/
void *heartbeat_thread(void *dummy) {

	// Control var
	sleep(1);
	int ret;
	int count = 240*2;

	// Init all the heartbeats
	int i = 0;
	for(i = 0; i < nHeartbeat; i++) {
		// Init the udp socket
		heartbeat[i].sockfd = init_udp_socket(1);

		// Set the remote address
		heartbeat[i].sockaddr = init_sock_addr( heartbeat[i].remoteIp, heartbeat[i].remotePort);

		// Get the socket address size to use on the sendto() function
		heartbeat[i].sockaddr_size = sizeof(heartbeat[i].sockaddr);

		// Init the counter
		heartbeat[i].counter = 0;
	}

	// Wait for NT COMM to start
	sleep(10);

	while(1) {
		// Send udp frame with the variables names
		if(count++ >= 240*2) {
			// Reset count
			count = 0;

			for(i = 0; i < nHeartbeat; i++) {
				if(strncmp(heartbeat[i].name, "NETSTATUS", sizeof("NETSTATUS")) == 0) {
					memset(heartbeat[i].udpPacket, 0, sizeof(heartbeat[i].udpPacket));
					ret = create_frame_netStatus(heartbeat[i].udpPacket, 1);
					if(ret < 0) {
						logger_write( ntcommlog,"<warn><heartbeat> Error creating frame for heartbeat ID(%d)", i);
					}
					else {
						if(ntcomm_values.debug == 1) {
							printf("hb packet: %s\n", heartbeat[i].udpPacket);
						}
						if(sendto(heartbeat[i].sockfd, heartbeat[i].udpPacket, (ret+1), 0, (struct sockaddr *)&heartbeat[i].sockaddr, heartbeat[i].sockaddr_size) < 0) {
							logger_write( ntcommlog,"<warn><heartbeat> Error sending heartbeat ID: %d (%d %s)", i, errno, strerror(errno));
						}
					}
				}
				else {
					logger_write( ntcommlog,"<warn><heartbeat> Unknown hearbeat (%s)", heartbeat[i].name);
				}
			}
		}
		// Send udp frame with values
		for(i = 0; i < nHeartbeat; i++) {
			if(strncmp(heartbeat[i].name, "NETSTATUS", sizeof("NETSTATUS")) == 0) {
				if(heartbeat[i].counter++ >= heartbeat[i].beatRate * 2) {
					heartbeat[i].counter = 0;
					memset(heartbeat[i].udpPacket, 0, sizeof(heartbeat[i].udpPacket));
					ret = create_frame_netStatus(heartbeat[i].udpPacket, 2);
					if(ret < 0) {
						logger_write( ntcommlog,"<warn><heartbeat> Error creating frame for heartbeat ID(%d)", i);
					}
					else {
						if(ntcomm_values.debug == 1) {
							printf("hb packet: %s\n", heartbeat[i].udpPacket);
						}
						if(sendto(heartbeat[i].sockfd, heartbeat[i].udpPacket, (ret+1), 0, (struct sockaddr *)&heartbeat[i].sockaddr, heartbeat[i].sockaddr_size) < 0) {
							logger_write(ntcommlog, "<warn><heartbeat> Error sending heartbeat ID: %d (%d %s)", i, errno, strerror(errno));
						}
					}
				}
			}
			else {
				logger_write( ntcommlog,"<warn><heartbeat> Unknown hearbeat (%s)", heartbeat[i].name);
			}
		}
		millisleep(500);
	}

	return NULL;
}
