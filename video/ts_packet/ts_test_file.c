/*
 * ts_test_file.c
 *
 *  Created on: 2015-1-23
 *      Author: shenwei
 */
#include <stdio.h>
#include <stdlib.h>
#include "ts_packet.h"



int32_t main(int32_t argc, char **argv){

	FILE *fp ;
	if(argc < 2){
		printf("%s xxx.ts\n", argv[0]);
		return 0;
	}

	fp = fopen(argv[1], "r");
	if(!fp){
		printf("open file:%s failed\n", argv[1]);
		return 0;
	}
	unsigned char buf[1316];
	int32_t n;

	struct ts_packet *ts = ts_packet_init();
	if(!ts){
		fclose(fp);
		return 0;
	}
	while((n = fread(buf, 1, sizeof(buf), fp)) == sizeof(buf)){
		ts_packet(ts, buf,  n,  188);
	}
	ts_packet_exit(ts);
	fclose(fp);

	return 0;
}
