/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : audio_sim.c
 *  Author       : wqshao
 *  Created Time : 2024-12-12 15:20:48
 *  Description  :
 *   利用fpcalc命令直接生产音频文件指纹信息对比
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//refer to alsa-utils-1.2.4
#define COMPOSE_ID(a,b,c,d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define WAV_RIFF        COMPOSE_ID('R','I','F','F')
#define WAV_RIFX        COMPOSE_ID('R','I','F','X')
#define WAV_WAVE        COMPOSE_ID('W','A','V','E')
#define WAV_FMT         COMPOSE_ID('f','m','t',' ')
#define WAV_DATA        COMPOSE_ID('d','a','t','a')

typedef struct{
	union {
		uint32_t	riff;
		char	riffFlag[4]; /* “RIFF” 资源交换文件标志 */
	}magic;
	uint32_t	fileLength;  /* 文件大小 */
	union {
		uint32_t	wave;
		char	waveFlag[4]; /* “wave” wav文件标志 */
	}type;
	char		fmtFlag[4];  /* “fmt” 波形格式标志,最后一位空格 */
	char		unused[4];
	uint16_t	fmtType;	 /* 格式种类(值为1时,表示数据为线性pcm编码) */
	uint16_t	channels;    /* 通道数,单声道为1,双声道为2 */
	uint16_t	rate;	     /* 采样率*/
	uint32_t	bps;		 /* 比特率(Byte率=采样频率*音频通道数*每次采样得到的样本位数/8) */
	uint16_t	blockSize;   /* 数据块长度(每个样本的字节数=通道数*每次采样得到的样本位数/8) */
	uint16_t	bits;		 /* 每个采样点的位数 */
	char		dataFlag[4]; /* “data”数据标志符 */
	uint32_t	voiceLength; /* pcm音频数据大小 */
}wav_head;

struct audio_sim {
	wav_head head1, head2;
	int ifd1, ifd2;
	int raw_size1, raw_size2;
	int raw_min_size;
};

static void dump_wav_info(wav_head *head)
{
	printf("### WAV Head info:\n\n");
	printf("   riffFlag : 0x%08x(%s)\n", head->magic.riff, head->magic.riffFlag);
	printf(" fileLength : %d\n", head->fileLength);
	printf("   waveFlag : 0x%08x(%s)\n", head->type.wave, head->type.waveFlag);
	printf("    fmtFlag : %s\n", head->fmtFlag);
	printf("     unused : %s\n", head->unused);
	printf("    fmtType : %d\n", head->fmtType);
	printf("   channels : %d\n", head->channels);
	printf("       rate : %d\n", head->rate);
	printf("        bps : %d\n", head->bps);
	printf("  blockSize : %d\n", head->blockSize);
	printf("       bits : %d\n", head->bits);
	printf("   dataFlag : %s\n", head->dataFlag);
	printf("voiceLength : %d\n", head->voiceLength);
	printf("Wav Head size: %ld Byte\n\n", sizeof(wav_head));
}

static uint32_t calculate_hamming_distance(uint32_t n1, uint32_t n2)
{
	uint32_t xor_res = n1 ^ n2;
	uint32_t distance = 0;

	//printf("===> func: %s, line: %d, n1=%d, n2=%d, xor_res=%d\n", __func__, __LINE__, n1, n2, xor_res);
	while (xor_res != 0) {
		distance += xor_res & 1;
		xor_res >>= 1;
		//printf("Current XOR result: %d, Distance: %d\n", xor_res, distance);
	}

	return distance;
}

static int dump_wav_raw_data(struct audio_sim *as)
{
	int i = 0;
	uint32_t data1, data2;
	uint32_t distance = 0;

	for (i = 0; i < as->raw_min_size; i += sizeof(uint32_t)) {
		if (-1 == read(as->ifd1, &data1, sizeof(data1))) {
			printf("%s Read fd1 (%s)\n", __func__, strerror(errno));
			return -1;
		}
		if (-1 == read(as->ifd2, &data2, sizeof(data2))) {
			printf("%s Read fd2 (%s)\n", __func__, strerror(errno));
			return -1;
		}

		//distance += pow((data1 - data2), 2);
		distance += calculate_hamming_distance(data1, data2);

		printf("===> func: %s, line: %d, data1=0x%08x, data2=0x%08x, distance=%d, i=%d\n", __func__, __LINE__, data1, data2, distance, i);
	}


	printf("===> func: %s, line: %d, i=%d, raw_min_size=%d\n", __func__, __LINE__, i, as->raw_min_size);
	printf("===> func: %s, line: %d, x=%f\n", __func__, __LINE__, sqrt(distance) / as->raw_min_size);
	printf("===> func: %s, line: %d, sqrt=%f, ----xx=%.2f\n", __func__, __LINE__, sqrt(distance), (1.0 - (sqrt(distance) / (float)as->raw_min_size)) * 100);

	return 0;
}

int main(int argc, const char *argv[])
{
	struct audio_sim as;
	const char *in_file1, *in_file2;
	struct stat fstat1, fstat2;

	if (argc != 3) {
		printf("Usage: %s <in_file1> <in_file2>\n", argv[0]);
		return -1;
	}

	in_file1 = argv[1];
	in_file2 = argv[2];
	printf("### in_file1:%s, in_file2:%s\n", in_file1, in_file2);

	memset(&as, 0, sizeof(as));

	// file1
	as.ifd1 = open(in_file1, O_RDONLY);
	if (-1 == as.ifd1) {
		printf("Cannot open file %s (%s)\n", in_file1, strerror(errno));
		exit(1);
	}
	if (stat(in_file1, &fstat1) == -1) {
		printf("Cannot get fstat %s (%s)\n", in_file1, strerror(errno));
		exit(1);
	}
	as.raw_size1 = fstat1.st_size - sizeof(as.head1);

	if(-1 == read(as.ifd1, &as.head1, sizeof(as.head1))) {
		printf("Cannot read %s (%s)\n", in_file1, strerror(errno));
		exit(1);
	}
	dump_wav_info(&as.head1);

	// file2
	as.ifd2 = open(in_file2, O_RDONLY);
	if (-1 == as.ifd2) {
		printf("Cannot open file %s (%s)\n", in_file2, strerror(errno));
		exit(1);
	}
	if (stat(in_file2, &fstat2) == -1) {
		printf("Cannot get fstat %s (%s)\n", in_file2, strerror(errno));
		exit(1);
	}
	as.raw_size2 = fstat2.st_size - sizeof(as.head2);

	if(-1 == read(as.ifd2, &as.head2, sizeof(as.head2))) {
		printf("Cannot read %s (%s)\n", in_file2, strerror(errno));
		exit(1);
	}
	dump_wav_info(&as.head2);

	as.raw_min_size = (as.raw_size1 <= as.raw_size2) ? as.raw_size1 : as.raw_size2;

	// check
	if(as.head1.type.wave != WAV_WAVE || as.head2.type.wave != WAV_WAVE) {
		printf("Not support, the file format is not wav.\n");
		exit(1);
	}
	if (as.head1.channels != as.head2.channels) {
		printf("Different channels, ch1=%d, ch2=%d\n", as.head1.channels, as.head2.channels);
		exit(1);
	}
	if (as.head1.rate != as.head2.rate) {
		printf("Different sampling rate, rate1=%d, rate2=%d\n", as.head1.rate, as.head2.rate);
		exit(1);
	}
	if (as.head1.bits != as.head2.bits) {
		printf("Different bits, bits1=%d, bits2=%d\n", as.head1.bits, as.head2.bits);
		exit(1);
	}

	// process
	printf("### File Type WAV, raw_min_size=%d\n", as.raw_min_size);
	dump_wav_raw_data(&as);

	close(as.ifd1);
	close(as.ifd2);

	return 0;
}
