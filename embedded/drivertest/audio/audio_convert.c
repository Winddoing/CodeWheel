/*#############################################################
 *     File Name	: audio_convert.c
 *     Author		: winddoing
 *     Created Time	: 2022年02月15日 星期二 13时48分23秒
 *     Description	:
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
		char	riffFlag[4];
	}magic;
	uint32_t	fileLength;
	union {
		uint32_t	wave;
		char	waveFlag[4];
	}type;
	char		fmtFlag[4];
	char		unused[4];
	uint16_t	fmtType;
	uint16_t	channels;
	uint16_t	rate;
	uint32_t	bps;
	uint16_t	blockSize;
	uint16_t	bits;
	char		dataFlag[4];
	uint32_t	voiceLength;
}wav_head;

struct audio_convert {
	wav_head head;
	int ifd, ofd;
	int if_size;
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

	printf("%s: start\n", __func__);

	//if ( -1 == write(act->ofd, &act->head, sizeof(act->head))) {
	//	printf("%s Write Head (%s)\n", __func__, strerror(errno));
	//	return -1;
	//}

	/*
	 *           LSB                           MSB
	 *           1st byte  2nd byte  3rd byte  4th byte   alignment
	 * S32_LE:   00000000  xxxxxxxx  xxxxxxxx  xxxxxxxx   32 bits
	 * S24_LE:   xxxxxxxx  xxxxxxxx  xxxxxxxx  00000000   32 bits
	 * S24_3LE:  xxxxxxxx  xxxxxxxx  xxxxxxxx             24 bits
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
	char *in_file, *out_file;
	struct stat fstat;

	if (argc != 3) {
		printf("Usage: %s <in_file> <out_file>", argv[0]);
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

	act.ofd = open(out_file, O_CREAT | O_RDWR | O_TRUNC);
	if (-1 == act.ofd) {
		printf("Cannot open file %s (%s)\n", out_file, strerror(errno));
		exit(1);
	}

	if (stat(in_file, &fstat) == -1) {
		printf("Cannot get fstat %s (%s)\n", in_file, strerror(errno));
		exit(1);
	}
	act.if_size = fstat.st_size;

	if(-1 == read(act.ifd, &act.head, sizeof(act.head))) {
		printf("Cannot read %s (%s)\n", in_file, strerror(errno));
		exit(1);
	}

	if(act.head.type.wave == WAV_WAVE) {
		printf("### File Type WAV\n");

		dump_wav_info(&act.head);

		act.raw_size = act.if_size - sizeof(act.head);
		s24_le_to_s24_3le(&act);

	} else {
		printf("### File Type RAW\n");

		act.raw_size = act.if_size;
		s24_le_to_s24_3le(&act);

	}

	close(act.ifd);
	close(act.ofd);
	
	return 0;
}
