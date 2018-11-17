/*
 * ts_packet.c
 *
 *  Created on: 2015-1-18
 *      Author: shenwei
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "ts_log.h"
#include "ts_buffer.h"
#include "ts_packet.h"

void ts_pid_free(struct ts_pid *pid){

	assert(pid);

	if(pid->adaptation_field){
		ts_buffer_free(pid->adaptation_field);
	}
	if(pid->data_payload){
		ts_buffer_free(pid->data_payload);
	}
	ts_free(pid);
}

struct ts_pid *ts_pid_new(){
	struct ts_pid *pid = ts_malloc(sizeof(*pid));
	if(!pid){
		ts_warn("malloc failed\n");
		return NULL;
	}
	memset(pid, 0, sizeof(*pid));
	pid->adaptation_field = ts_buffer_new();
	if(!pid->adaptation_field){
		ts_warn("malloc adaptation_field failed\n");
		goto error;
	}
	pid->data_payload = ts_buffer_new();
	if(!pid->data_payload){
		ts_warn("malloc data_payload failed\n");
		goto error;
	}
	return pid;

error:
	ts_pid_free(pid);
	return NULL;
}

int32_t ts_pid_init(struct ts_pid *ts_pid, int32_t pid, int32_t type){
	assert(ts_pid && pid >= 0 && pid < 8192);
	ts_pid->pid = pid;
	ts_pid->type = type;
	ts_pid->pass = 1;
	return 0;
}

struct ts_packet *ts_packet_new(){
	struct ts_packet *ts = ts_malloc(sizeof(*ts));
	if(!ts){
		ts_warn("malloc failed\n");
		return NULL;
	}
	memset(ts, 0, sizeof(*ts));
	return ts;
}

void ts_packet_free(struct ts_packet *ts){
	assert(ts);
	ts_free(ts);
}

struct ts_packet *ts_packet_init(){
	struct ts_packet *ts = ts_packet_new();
	if(!ts){
		return NULL;
	}
	int i;
	for(i = 0; i < 8192; i++){
		ts->pids[i] = NULL;
	}
	ts->pids[0] = ts_pid_new();
	ts_pid_init(ts->pids[0], 0, TS_PAT_PID);
	ts->psi_parse_timeout = 60;
	gettimeofday(&ts->psi_start_tv, NULL);
	return ts;
}

void ts_program_free(struct ts_pmt *program){
	assert(program);
	struct es_info_header *tmp, *es_info;
	es_info = program->next;
	while(es_info){
		tmp = es_info;
		es_info = tmp->next;
		ts_free(tmp);
	}
}

void ts_packet_exit(struct ts_packet *ts){

	assert(ts);
	int32_t i;
	for(i = 0 ; i < 8192; i++){
		if(ts->pids[i]){
			ts_pid_free(ts->pids[i]);
		}
	}
	if(ts->programs){
		for(i = 0; i < ts->program_num; i++){
			ts_program_free(&ts->programs[i]);
		}
		ts_free(ts->programs);
	}
	ts_packet_free(ts);
}


int32_t ts_pat_parse(struct ts_packet *ts, struct ts_pid *pid){
	assert(ts && pid);

	int32_t i;
	unsigned char *p;
	int32_t size = ts_buffer_size(pid->data_payload);
	p = pid->data_payload->data;
	if(size <= 8){
		pid->data_done = 0;
		return 0;
	}
	int32_t table_id = p[0];
	if(table_id != 0x0){
		pid->data_done = 0;
		ts_warn("PAT table id:%d != 0x0\n", table_id);
		return 0;
	}
	int32_t section_length = ((p[1]&0xF) << 8) | p[2];
	if(section_length > size - 3){
		pid->data_done = 0;
		return 0;
	}
	ts->program_num = (section_length - 4 - 5)/4; //???
	//ts->program_num = (section_length - 4(CRC的4Byte) - 5(section_lengt以后的5Byte))/4
	ts->programs = ts_malloc(sizeof(struct ts_pmt)*ts->program_num);
	if(!ts->programs){
		ts_warn("malloc failed\n");
		return 0;
	}
	memset(ts->programs, 0 , sizeof(struct ts_pmt)*ts->program_num);

	p = p + 8;
	for(i = 0; i < ts->program_num; i++){
		ts->programs[i].service_id = (p[0] << 8) | p[1];
		ts->programs[i].pid = (((p[2]&0x1F) << 8) | p[3])&0x1fff;
		ts_info(" (%d) service id:%d pid:%u\n", i + 1,  ts->programs[i].service_id, ts->programs[i].pid);
		p = p + 4;
	}

	ts->pat_done = 1;
	/*FIXME: no check crc*/

	return 1;
}

int32_t ts_add_pmt(struct ts_packet *ts){
	assert(ts);
	int32_t i;
	int32_t pid;
	for(i = 0; i < ts->program_num; i++){
		pid = ts->programs[i].pid;
		ts->pids[pid] = ts_pid_new();
		if(!ts->pids[pid]){
			return 0;
		}
		if(ts->programs[i].service_id == 0){
			continue;
		}
		ts_pid_init(ts->pids[pid], pid, TS_PMT_PID);
		ts->pids[pid]->service_id = ts->programs[i].service_id;
	}
	return 1;
}

struct es_info_header *ts_es_info_header_new(){
	struct es_info_header *es = ts_malloc(sizeof(*es));
	if(!es){
		ts_warn("malloc failed\n");
		return NULL;
	}
	memset(es, 0, sizeof(*es));
	return es;
}

void ts_es_info_header_free(struct es_info_header *es){
	assert(es);
	ts_free(es);

}

int32_t ts_add_es_info(struct ts_packet *ts, int32_t service_id, int32_t stream_type, int32_t pid){

	assert(ts && pid >=0 && pid < 8192);

	struct es_info_header *es = ts_es_info_header_new();
	if(!es){
		return 0;
	}
	int32_t i;
	es->stream_type = stream_type;
	es->pid = pid;
	struct ts_pmt *program = NULL;
	for(i = 0; i < ts->program_num; i++){
		if(ts->programs[i].service_id == service_id){
			program = &ts->programs[i];
			break;
		}
	}
	if(program){
		es->next = program->next;
		program->next = es;
	}else{
		ts_es_info_header_free(es);
		ts_warn("No find service id:%d in programs table\n", service_id);
		return 0;
	}
	return 1;
}

int32_t ts_pmt_done(struct ts_packet *ts, int32_t service_id){
	assert(ts);

	int32_t i;
	int32_t done_num = 0;
	for(i = 0; i < ts->program_num; i++){
		if(ts->programs[i].service_id == service_id){
			ts->programs[i].over = 1;
		}

		if(ts->programs[i].over){
			done_num++;
		}
	}
	/*exclude:service id == 0 */
	if(done_num){
		ts_info("all pmt pid parsed\n");
		ts->pmts_done = 1;
	}
	return 1;
}

int32_t ts_pmt_parse(struct ts_packet *ts, struct ts_pid *pid){
	assert(ts && pid);
	int32_t service_id;
	unsigned char *p;
	int32_t size = ts_buffer_size(pid->data_payload);
	p = pid->data_payload->data;
	if(size <= 12){
		pid->data_done = 0;
		return 0;
	}
	int32_t table_id = p[0];
	if(table_id != 0x02){
		pid->data_done = 0;
		ts_warn("PMT table id:%d != 0x02\n", table_id);
		return 0;
	}
	int32_t section_length = (((p[1]&0xF) << 8) | p[2])&0xfff;
	if(section_length > size - 3){
		pid->data_done = 0;
		return 0;
	}
	int32_t program_info_length;
	/*CRC:4 bytes,section head:9 bytes*/
	if(section_length - 4 - 9 > 0){
		program_info_length = (((p[10]&0xF) << 8)|p[11])&0xfff;
	}else{
		return 0;
	}
	if(section_length - 4 - 9 - program_info_length <= 0){
		ts_warn("PMT no elementary_PID\n");
		return 0;
	}
	service_id = (p[3] << 8)|p[4];
	unsigned char *end = p + section_length - 4;
	p = p + 12 + program_info_length;
	int32_t ES_info_length;
	int32_t stream_type;
	int32_t elementary_PID;
	int32_t count = 0;
	while(p < end){
		stream_type = p[0]&0xff;
		elementary_PID = (((p[1]&0x1F)<< 8)|p[2])&0x1fff;
		ES_info_length = (((p[3]&0xF)<< 8)|p[4])&0xfff;
		count++;
		ts_info(" (%d) service id:%d pid:%d stream type:0x%x \n", count,  service_id,  elementary_PID,  stream_type);
		ts_add_es_info(ts,  service_id,  stream_type,  elementary_PID);
		p = p + 5 + ES_info_length;
	}

	ts_pmt_done(ts, service_id);
	/*FIXME: no check crc*/

	return 1;
}

int32_t ts_add_elementary_PID(struct ts_packet *ts){
	assert(ts);

	int32_t i;
	int32_t pid;
	struct es_info_header *es;
	for(i = 0; i < ts->program_num; i++){
		if(ts->programs[i].service_id == 0){
			continue;
		}
		es = ts->programs[i].next;
		while(es){
			pid = es->pid;
			ts->pids[pid] = ts_pid_new();
			if(!ts->pids[pid]){
				return 0;
			}
			if(es->stream_type == 0x02 || es->stream_type == 0x1b){
				ts_pid_init(ts->pids[pid], pid, TS_VIDEO_PID );

			}else if(es->stream_type == 0x04 || es->stream_type == 0x0f){
				ts_pid_init(ts->pids[pid], pid, TS_AUDIO_PID);
			}
			ts->pids[pid]->service_id = ts->programs[i].service_id;
			es = es->next;
		}
	}
	return 1;
}

int32_t ts_psi_is_done(struct ts_packet *ts){
	assert(ts);
	return ts->pmts_done;
}

int32_t ts_psi_is_timeout(struct ts_packet *ts){
	assert(ts);
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec > ts->psi_start_tv.tv_sec + ts->psi_parse_timeout);
}

int32_t ts_pes_parse( struct ts_packet *ts, struct ts_pid *pid){
	assert(ts && pid);

	int32_t PES_packet_length = 0;
	int32_t PES_header_data_length;
	int32_t PTS_DTS_flags;
	unsigned char *p = pid->data_payload->data;
	int32_t size = ts_buffer_size(pid->data_payload);
	if(size < 5){
		pid->data_done = 0;
		pid->data_start = 0;
		ts_buffer_init(pid->data_payload);
		return 0;
	}
	if(p[0] !=  0 && p[1] != 0 && p[2] != 0x01){
		pid->data_done = 0;
		pid->data_start = 0;
		ts_buffer_show(pid->data_payload, 32);
		ts_buffer_init(pid->data_payload);
		ts_warn("is not pes start\n");
		return 0;
	}

	PES_packet_length = ((p[4] << 8) | p[5]);
	PTS_DTS_flags = (p[7] >> 6) & 0x03;
	PES_header_data_length = p[8];
	if(size < PES_header_data_length + 9){
		ts_warn("pid:%d PES_header_data_length:%d too long than data size:%d\n", pid->pid ,PES_packet_length, size);
		ts_buffer_show(pid->data_payload, 32);
		return 0;
	}

	p = p + 9;
	if(PTS_DTS_flags == 2 && size >= 14){
		pid->pts = ((p[0] >> 1)&0x7) << 30;
		pid->pts |= (p[1]&0xff) << 22;
		pid->pts |= ((p[2] >> 1)&0x7F) << 15;
		pid->pts |= (p[3]&0xff) << 7;
		pid->pts |= ((p[4] >> 1)&0x7F) ;
	}else if(PTS_DTS_flags == 3 && size >= 19){
		pid->pts = ((p[0] >> 1)&0x7) << 30;
		pid->pts |= (p[1]&0xff) << 22;
		pid->pts |= ((p[2] >> 1)&0x7F) << 15;
		pid->pts |= (p[3]&0xff) << 7;
		pid->pts |= ((p[4] >> 1)&0x7F) ;

		p  = p + 5;
		pid->dts = ((p[0] >> 1)&0x7) << 30;
		pid->dts |= (p[1]&0xff) << 22;
		pid->dts |= ((p[2] >> 1)&0x7F) << 15;
		pid->dts |= (p[3]&0xff) << 7;
		pid->dts |= ((p[4] >> 1)&0x7F) ;
	}else{
		ts_warn("pid:%d pes length error\n", pid->pid);
	}

	//ts_info("PES_header_data_length:%d data size:%d pes length:%d\n", PES_header_data_length, size, PES_packet_length);
	p = pid->data_payload->data + 9 + PES_header_data_length;

	int32_t output_size = 0;
	if(PES_packet_length > 0){
		if(6 + PES_packet_length > size){
			ts_info("PES_header_data_length:%d data size:%d pes length:%d type:%d\n", PES_header_data_length, size, PES_packet_length, pid->type);
			//return 0;
			output_size = size - ( 9 + PES_header_data_length);
		}else{
			output_size = PES_packet_length;
		}
	}else{
		output_size = size - ( 9 + PES_header_data_length);
	}
	if(output_size > 0 && ts->output_callback){
		ts->output_callback(pid, p, output_size);
	}
	return 0;
}

/*just jump adaptation or point field*/
int32_t ts_point_field(struct ts_pid *pid, unsigned char *buf, int32_t size){
	assert(pid && buf && size > 0);
	int32_t left_size = 0;
	int32_t payload_unit_start_indicator = (buf[1]>>6)&0x1;

	if(payload_unit_start_indicator){
		pid->field_done = 0;
		pid->field_length = buf[4]&0xff;
		//ts_buffer_init(pid->field);

		if(size - 5 <= pid->field_length){
			ts_warn("field length:%d error\n", pid->field_length);
			pid->field_done = 0;
		}else{
			//ts_buffer_write(pid->field, buf + 5, size - 5);
			left_size = size - 5 - pid->field_length;
			pid->field_done = 1;
		}
	}
	return left_size;
}

int32_t ts_adaptation_field(struct ts_pid *pid, unsigned char *buf, int32_t size){
	assert(pid && buf && size > 0);
	int32_t left_size = 0;

	pid->field_done = 0;
	pid->field_length = buf[4]&0xff;
	//ts_buffer_init(pid->field);

	if(size - 5 <= pid->field_length){
		ts_warn("field length:%d error\n", pid->field_length);
		pid->field_done = 0;
	}else{
		//ts_buffer_write(pid->field, buf + 5, size - 5);
		left_size = size - 5 - pid->field_length;
		pid->field_done = 1;
	}
	return left_size;
}

int32_t ts_payload(struct ts_packet *ts, struct ts_pid *pid, unsigned char *buf, int32_t offset){

	assert(ts && pid && buf);

	int32_t left_size = 0;
	int32_t payload_unit_start_indicator = (buf[1]>>6)& 0x1;
	left_size = 188 - offset;

	if(payload_unit_start_indicator){

		/*this psi/pes start  is the prev one end*/
		if(ts_buffer_size(pid->data_payload) > 3){
			pid->data_done = 1;
			pid->data_start = 0;
			switch(pid->type){
			case TS_VIDEO_PID:
			case TS_AUDIO_PID:
				//printf("%s pid->type =%d\n", __func__, pid->type);
				ts_pes_parse(ts, pid);
				break;
			case TS_PAT_PID:
				printf("%s pid->type =%d TS_PAT_PID\n", __func__, pid->type);
				ts_pat_parse(ts, pid);
				if(pid->data_done){
					ts_add_pmt(ts);
					return 0;
				}
				break;
			case TS_PMT_PID:
				printf("%s pid->type =%d TS_PMT_PID\n", __func__, pid->type);
				ts_pmt_parse(ts, pid);
				if(pid->data_done){
					return 0;
				}
				break;
			}
		}
		pid->data_done = 0;
		pid->data_start = 1;
		ts_buffer_init(pid->data_payload);
		ts_buffer_write(pid->data_payload, buf + offset, left_size);
	}else if(pid->data_start){
		ts_buffer_write(pid->data_payload, buf + offset, left_size);
	}
	return 0;
}

int32_t ts_data(struct ts_packet *ts, struct ts_pid *pid, unsigned char *buf){

	assert(ts && pid && buf);

	int32_t left_size = 0;
	int32_t adaptation_field_control = (buf[3] >> 4) & 0x3;

	if(adaptation_field_control == 1){
		ts_payload(ts, pid, buf,  4);
	}else if(adaptation_field_control == 2){
		ts_adaptation_field(pid, buf, 188);
	}else if(adaptation_field_control == 3){
		left_size = ts_adaptation_field(pid, buf, 188);
		if(left_size > 0){
			ts_payload(ts, pid, buf,  188 - left_size);
		}
	}
	return 1;
}


int32_t ts_psi(struct ts_packet *ts, struct ts_pid *pid, unsigned char *buf){
	assert(ts && pid && buf);
	int32_t left_size = 0;
	/* 判断自适应区的大小 */
	left_size = ts_point_field(pid, buf, 188);
	if(left_size > 0){
		ts_payload(ts, pid, buf,  188 - left_size);
	}else{
		ts_payload(ts, pid, buf,  4);
	}

	return 1;
}

struct ts_pid *ts_get_pid(struct ts_packet *ts, int32_t pid){
	assert(pid >= 0 && pid < 8192);
	return ts->pids[pid];
}

int32_t ts_av_packet(struct ts_packet *ts, unsigned char *buf, int32_t size, int32_t ts_len){

	assert(ts && buf && ts_len >= 188);

	int32_t i;
	int32_t pid;
	struct ts_pid *ts_pid;
	unsigned char *p;

	for(i = 0; i + 188 <= size; i = i + ts_len){
		p = buf + i;
		if(p[0] != 0x47){
			ts_warn("ts no sync\n");
			return 0;
		}
		pid = (((p[1]&0x1f) << 8)|p[2])&0x1fff;
		ts_pid = ts_get_pid(ts, pid);
		if(!ts_pid){
			continue;
		}
		switch(ts_pid->type){
		case TS_VIDEO_PID:
		case TS_AUDIO_PID:
			ts_data(ts, ts_pid, p);
			break;
		}
	}

	return size;
}

/*
 * demux ts with pat and pmt,
 * no  parse sdt and other psi/si table
 * */
int32_t ts_psi_packet(struct ts_packet *ts, unsigned char *buf, int32_t size, int32_t ts_len){

	assert(ts && buf && ts_len >= 188);

	int32_t i;
	int32_t pid = 0;
	struct ts_pid *ts_pid;
	unsigned char *p;

	for(i = 0; i + 188 <= size; i = i + ts_len){
		p = buf + i;
		if(p[0] != 0x47){
			ts_warn("ts no sync\n");
			return 0;
		}
		if(ts_psi_is_done(ts)){
			ts_info("parse psi success\n");
			return 1;
		}
		if(ts_psi_is_timeout(ts)){
			ts_info("parse psi timeout\n");
			return 1;
		}
		pid = (((p[1]&0x1f) << 8) | p[2]) & 0x1fff;
		ts_pid = ts_get_pid(ts, pid);
		if(!ts_pid){
			continue;
		}
		if(ts_pid->data_done){
			continue;
		}
		switch(ts_pid->type){
		case TS_PAT_PID:
		case TS_PMT_PID:
			ts_psi(ts, ts_pid, p);
			break;
		default:
			break;
		}
	}

	return size;
}

void ts_output_enable(struct ts_packet *ts, int32_t enable){
	assert(ts);
	ts->output_enable = enable;
}

void ts_output_config(struct  ts_packet *ts){
	assert(ts);
	if(ts->config_callback){
		ts->config_callback(ts);
	}
}

int32_t ts_set_pid_filter_table(struct ts_packet *ts, int32_t pids[], int32_t size){
	assert(ts && pids && size > 0);
	ts->filter_pids = ts_malloc(sizeof(int32_t)*size);
	if(!ts->filter_pids){
		return 0;
	}
	memcpy(ts->filter_pids, pids, sizeof(int32_t)*size);
	ts->filter_pid_num = size;
	return 1;

}

int32_t *ts_get_pid_filter_table(struct ts_packet *ts){
	assert(ts);
	return ts->filter_pids;
}

int32_t ts_set_service_id_filter_table(struct ts_packet *ts, int32_t service_ids[], int32_t size){
	assert(ts && service_ids && size > 0);
	ts->filter_service_ids = ts_malloc(sizeof(int32_t)*size);
	if(!ts->filter_service_ids){
		return 0;
	}
	memcpy(ts->filter_service_ids, service_ids, sizeof(int32_t)*size);
	ts->filter_service_id_num = size;
	return 1;
}

int32_t *ts_get_service_id_filter_table(struct ts_packet *ts){
	assert(ts);
	return ts->filter_service_ids;
}

void ts_set_filter_program(struct ts_packet *ts,int32_t service_id){
	assert(ts);

	int32_t j;
	int32_t find = 0;
	struct ts_pid *ts_pid;
	struct es_info_header *es_info, *tmp;
	for(j = 0; j < ts->program_num; j++){
		if(service_id != ts->programs[j].service_id){
			continue;
		}
		ts->programs[j].pass = 0;
		es_info = ts->programs[j].next;
		while(es_info){
			tmp = es_info;
			es_info = tmp->next;
			ts_pid = ts_get_pid(ts, tmp->pid);
			if(ts_pid){
				ts_pid->pass = 0;
			}
		}
		find = 1;
		break;
	}
	if(!find){
		ts_warn("no found service id :%d in stream\n", service_id);
	}
}

void ts_output_filter(struct ts_packet *ts){

	assert(ts);
	int32_t i;
	int32_t pid;
	int32_t *pids = ts_get_pid_filter_table(ts);
	int32_t *service_ids = ts_get_service_id_filter_table(ts);

	struct ts_pid *ts_pid;
	for(i = 0; i < ts->filter_pid_num; i++){
		pid = pids[i];
		ts_pid = ts_get_pid(ts, pid);
		if(ts_pid){
			ts_pid->pass = 0;
		}else{
			ts_warn("no found pid:%d in stream\n", pid);
		}
	}
	for(i = 0; i < ts->filter_service_id_num; i++){
		ts_set_filter_program(ts, service_ids[i]);
	}
}

/*
 * @ts  struct ts_packet
 * @buf input ts buffer
 * @size input ts size
 * @ts_len ts packet length as 188, 204
 * */
int32_t ts_packet(struct ts_packet *ts, unsigned char *buf, int32_t size, int32_t ts_len){

	assert(ts && buf && ts_len >= 188);
	if(!ts->pmts_done){
		ts_psi_packet(ts, buf, size, ts_len);
		if(ts_psi_is_done(ts)|| ts_psi_is_timeout(ts)){
			ts_add_elementary_PID(ts);

			/*config and filter av output here*/
			ts_output_enable(ts, 1);
			ts_output_filter(ts);
			ts_output_config(ts);

			ts->pmts_done = 1;
		}
	}

	if(ts->output_enable){
		ts_av_packet(ts, buf, size, ts_len);
	}
	return 0;
}


void ts_register_output_callback(struct ts_packet *ts, output_handler_t callback){
	assert(ts);
	ts->output_callback = callback;
}

void ts_unregister_output_callback(struct ts_packet *ts){
	assert(ts);
	ts->output_callback = NULL;
}

int32_t ts_default_config(struct ts_packet *ts){
	int32_t j;
	struct ts_pid *ts_pid;
	struct es_info_header *es_info, *tmp;
	for(j = 0; j < ts->program_num; j++){

		if(!ts->programs[j].pass){
			continue;
		}
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
			}
		}
	}
	return 0;
}

void ts_register_config_callback(struct ts_packet *ts, config_handler_t callback){
	assert(ts);
	ts->config_callback = callback;

}

void ts_unregister_config_callback(struct ts_packet *ts){
	assert(ts);
	ts->config_callback = NULL;
}


