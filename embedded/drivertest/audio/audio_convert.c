/*#############################################################
 *     File Name	: audio_convert.c
 *     Author		: winddoing
 *     Created Time	: 2022年02月15日 星期二 13时48分23秒
 *     Description	:
 *			http://www.topherlee.com/software/pcm-tut-wavformat.html
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

struct audio_convert {
	wav_head head;
	int ifd, ofd;
	int raw_size;
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


static int s24_le_to_s24_3le(struct audio_convert *act)
{
	int i = 0;
	char buf[4];
	int of_raw_sz = 0;
	int of_duration_time = 0;
	wav_head ohead;

	printf("%s: start\n", __func__);

	of_raw_sz = act->raw_size * 3 / 4;
	of_duration_time = (of_raw_sz * 8) /
		(act->head.bits * act->head.channels * act->head.rate);

	memcpy(&ohead, &act->head, sizeof(ohead));

	ohead.blockSize = (act->head.bits * act->head.channels / 8);  //原头部为8,不调整无法播放
	ohead.bps = of_raw_sz / of_duration_time;
	ohead.voiceLength = of_raw_sz;

	dump_wav_info(&ohead);

	if ( -1 == write(act->ofd, &ohead, sizeof(ohead))) {
		printf("%s Write Head (%s)\n", __func__, strerror(errno));
		return -1;
	}

	/*
	 *           MSB                           LSB
	 *           4th byte  3rd byte  2nd byte  1st byte   alignment
	 * S24_LE:   00000000  xxxxxxxx  xxxxxxxx  xxxxxxxx   32 bits
	 * S24_3LE:			   xxxxxxxx  xxxxxxxx  xxxxxxxx   24 bits
	 */
	for (i = 0; i < act->raw_size; i = i + 4) {
		/* 1. read 4 bytes S24_LE [xx xx xx 00] */
		if (-1 == read(act->ifd, buf, 4)) {
			printf("%s Read (%s)\n", __func__, strerror(errno));
			return -1;
		}

		if (0) {
			int j=0;
			for (j = 0; j < 4; j++) {
				printf("== buf[%d]=%02x\n", j, buf[j]);
			}
		}

		/* 2. write lower 3 bytes [xx xx xx] */
		if ( -1 == write(act->ofd, buf, 3)) {
			printf("%s Write (%s)\n", __func__, strerror(errno));
			return -1;
		}
	}
	printf("%s: end\n", __func__);

	return 0;
}

int main(int argc, const char *argv[])
{
	struct audio_convert act;
	const char *in_file, *out_file;
	struct stat fstat;
	int fsize = 0;

	if (argc != 3) {
		printf("Usage: %s <in_file> <out_file>\n", argv[0]);
		printf(" PCM data S24_LE to S24_3LE\n");
		return -1;
	}

	in_file = argv[1];
	out_file = argv[2];

	printf("### in_file:%s, out_file:%s\n", in_file, out_file);

	act.ifd = open(in_file, O_RDONLY);
	if (-1 == act.ifd) {
		printf("Cannot open file %s (%s)\n", in_file, strerror(errno));
		exit(1);
	}

	act.ofd = open(out_file, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	if (-1 == act.ofd) {
		printf("Cannot open file %s (%s)\n", out_file, strerror(errno));
		exit(1);
	}

	if (stat(in_file, &fstat) == -1) {
		printf("Cannot get fstat %s (%s)\n", in_file, strerror(errno));
		exit(1);
	}
	fsize = fstat.st_size;

	if(-1 == read(act.ifd, &act.head, sizeof(act.head))) {
		printf("Cannot read %s (%s)\n", in_file, strerror(errno));
		exit(1);
	}

	if(act.head.type.wave == WAV_WAVE) {
		printf("### File Type WAV\n");

		dump_wav_info(&act.head);

		act.raw_size = fsize - sizeof(act.head);
		s24_le_to_s24_3le(&act);

	} else {
		printf("### File Type RAW\n");

		act.raw_size = fsize;
		s24_le_to_s24_3le(&act);

	}

	close(act.ifd);
	close(act.ofd);

	return 0;
}
