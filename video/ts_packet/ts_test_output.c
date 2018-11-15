/*
 * ts_test_output.c
 *
 *  Created on: 2015-1-23
 *      Author: shenwei
 */


#include <stdio.h>
#include <stdlib.h>
#include "ts_packet.h"

int32_t ts_out_es_config(struct ts_packet *ts){
	int32_t j;
	char filename[64];
	FILE *fp = NULL;
	struct ts_pid *ts_pid;
	struct es_info_header *es_info, *tmp;
	for(j = 0; j < ts->program_num; j++){


		/*
		 * struct priv_data *priv_data = NULL;
		 * do something
		 * */

		es_info = ts->programs[j].next;
		while(es_info){
			tmp = es_info;
			es_info = tmp->next;
			ts_pid = ts_get_pid(ts, tmp->pid);
			if(ts_pid && ts_pid->pass){

				/*
				 * ts_pid->priv_data = priv_data;
				 * */
				snprintf(filename, sizeof(filename), "%s_%d.es",ts_pid->type == TS_VIDEO_PID?"video":"audio", ts_pid->pid);
				fp = fopen(filename , "w+");
				ts_pid->priv_data = fp;
			}
		}
	}
	return 0;
}


int32_t ts_exit_es_config(struct ts_packet *ts){
	int32_t j;
	FILE *fp = NULL;
	struct ts_pid *ts_pid;
	struct es_info_header *es_info, *tmp;
	for(j = 0; j < ts->program_num; j++){


		/*
		 * struct priv_data *priv_data = NULL;
		 * do something
		 * */

		es_info = ts->programs[j].next;
		while(es_info){
			tmp = es_info;
			es_info = tmp->next;
			ts_pid = ts_get_pid(ts, tmp->pid);
			if(ts_pid && ts_pid->pass){

				/*
				 * ts_pid->priv_data = priv_data;
				 * */
				fp = (FILE *)ts_pid->priv_data;
				if(fp){
					fclose(fp);
				}
			}
		}
	}
	return 0;
}

int32_t es_output(struct ts_pid *pid, unsigned char *buf, int32_t size){
	int n = 0;
	FILE *fp = (FILE *)pid->priv_data;
	if(fp){
		n = fwrite(buf, 1, size, fp);
	}
	return n;
}

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
	ts_register_output_callback(ts, es_output);
	ts_register_config_callback(ts, ts_out_es_config);
	while((n = fread(buf, 1, sizeof(buf), fp)) == sizeof(buf)){
		ts_packet(ts, buf,  n,  188);
	}
	ts_exit_es_config(ts);
	ts_unregister_output_callback(ts);
	ts_unregister_config_callback(ts);
	ts_packet_exit(ts);
	fclose(fp);

	return 0;
}
