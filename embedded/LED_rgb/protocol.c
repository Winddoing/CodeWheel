#include <stdio.h>
#include <string.h>
#include "protocol.h"

static const char preamble[] = PREAMBLE;
static const char dev_id[] = DEVICE_ID;
static const char version[] = VERSION;

static void checksum(char *buf)
{
	int i;
	int len = VERSION_POS+(buf[LEN_POS] << 8)+(buf[LEN_POS+1]);
	unsigned char check = 0;
	for(i=VERSION_POS;i<len-1;i++){
		check += buf[i];
	}
	buf[len-1] = check;
}
int check_buf(char *buf, int len)
{
	char check = 0;
	int data_len = VERSION_POS+(buf[LEN_POS] << 8)+(buf[LEN_POS+1]);
	int i;
	if(len < VERSION_POS)return -1;//msg too short
	if(data_len>len)return -1;//msg too short
	for(i=VERSION_POS;i<data_len-1;i++){
		check += buf[i];
	}
	if(check != buf[data_len-1])return -2;//checksum error
	for(i=0;i<16;i++){
		if(buf[DATA_POS+i] != dev_id[i])return -3;//dev_id error
	}
	return 0;
}
int str_login(char *buf)
{
	const char login_len[] = {0x00, 0x17};
	memcpy(&buf[PREAMBLE_POS], preamble, sizeof(preamble));
	memcpy(&buf[LEN_POS], login_len, sizeof(login_len));
	memcpy(&buf[VERSION_POS], version, sizeof(version));
	buf[CMD_POS] = 0x00;
	buf[CMD_POS+1] = CMD_LOGIN;
	memcpy(&buf[DATA_POS], dev_id, sizeof(dev_id));
	checksum(buf);
	return VERSION_POS + (buf[LEN_POS]<<8)+buf[LEN_POS+1];
}
int str_report(char *buf)
{
	const char login_len[] = {0x00, 0x1A};
	memcpy(&buf[PREAMBLE_POS], preamble, sizeof(preamble));
	memcpy(&buf[LEN_POS], login_len, sizeof(login_len));
	memcpy(&buf[VERSION_POS], version, sizeof(version));
	buf[CMD_POS] = 0x00;
	buf[CMD_POS+1] = CMD_REPORT_STATUS;
	memcpy(&buf[DATA_POS], dev_id, sizeof(dev_id));
	get_led_state(&buf[DATA_POS+sizeof(dev_id)]);
	checksum(buf);
	return VERSION_POS + (buf[LEN_POS]<<8)+buf[LEN_POS+1];
}
