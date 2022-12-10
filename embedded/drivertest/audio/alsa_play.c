/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : alsa_play.c
 *  Author       : wqshao
 *  Created Time : 2022-12-09 14:02:54
 *  Description  :
 *      build gcc alsa_play.c `pkg-config --libs --cflags alsa`
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <alsa/asoundlib.h>
#include <stdlib.h>

//需要从wav文件中读取的三个参数
typedef struct {
	snd_pcm_t* handle;
	snd_pcm_hw_params_t* hw_params;

	//配置参数，解析WAV文件
	snd_pcm_format_t format;
	unsigned int channels;
	unsigned int rate;
} snd_pcm_params;

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

static int parse_wav_file(int fd, wav_head *head)
{
	assert(fd != -1);
	assert(head != NULL);

	lseek(fd, 0, SEEK_SET);

	if(-1 == read(fd, head, sizeof(wav_head))) {
		printf("Cannot read %d (%s)\n", fd, strerror(errno));
		exit(1);
	}

	if(head->type.wave == WAV_WAVE) {
		dump_wav_info(head);
		return 0;
	}

	return -1;
}

static void setup_audio_params(wav_head *head, snd_pcm_params* pcm_params)
{
	pcm_params->channels = head->channels;
	pcm_params->rate	 = head->rate;

	switch (head->bits) {
	case 8:
		pcm_params->format = SND_PCM_FORMAT_U8;
		break;

	case 16:
		pcm_params->format = SND_PCM_FORMAT_S16_LE;
		break;

	default:
		printf("FIXME: add more format, bits=%d\n", head->bits);
		exit(1);
	}
	printf("%s: rate=%d, channels=%d, format=%d(%dbits)\n", __func__, pcm_params->rate,
			pcm_params->channels, pcm_params->format, head->bits);
}

static int pcm_open(snd_pcm_params* pcm_params)
{
	int ret = 0;

	//1. 打开设备
	if ((ret = snd_pcm_open(&pcm_params->handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		printf("open pcm device error:%s\n", snd_strerror(ret));
		return -1;
	}

	return 0;
}

static int pcm_set_params(snd_pcm_params* pcm_params)
{
	int ret, val, dir;
	snd_pcm_uframes_t frames;

	//2. 给参数分配空间,并用hw_param(从wav头中分析出的参数)初始化
	snd_pcm_hw_params_alloca(&pcm_params->hw_params);
	snd_pcm_hw_params_any(pcm_params->handle, pcm_params->hw_params);
	snd_pcm_hw_params_set_access(pcm_params->handle, pcm_params->hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	//snd_pcm_hw_params_set_format(pcm_params->handle, pcm_params->hw_params, SND_PCM_FORMAT_S16_LE); //last param get from wav file

	snd_pcm_hw_params_set_format(pcm_params->handle, pcm_params->hw_params, pcm_params->format);
	//snd_pcm_hw_params_set_channels(handle, hw_params, 2);
	snd_pcm_hw_params_set_channels(pcm_params->handle, pcm_params->hw_params, pcm_params->channels); //last param get from wav file
	printf("hw_params: format=%d, channels=%d", pcm_params->format, pcm_params->channels);
	val = 44100;
	snd_pcm_hw_params_set_rate_near(pcm_params->handle, pcm_params->hw_params, &val, &dir);
	frames = 32 * 4;
	//snd_pcm_hw_params_set_period_size_near(pcm_params->handle, pcm_params->hw_params, &frames, &dir);

	//3. set param to driver
	if ((ret = snd_pcm_hw_params(pcm_params->handle, pcm_params->hw_params)) < 0) {
		printf("set hw params error:%s", snd_strerror(ret));
		return -1;
	}
	return 0;
}

static int pcm_paly(snd_pcm_params* pcm_params)
{
	return 0;
}

static int pcm_close(snd_pcm_params* pcm_params)
{
	snd_pcm_drain(pcm_params->handle);
	snd_pcm_close(pcm_params->handle);

	return 0;
}

int main(int argc, char *argv[])
{
	int i, fd;
	int ret, dir, buf_size;
	unsigned int val, val2;
	char* buffer;
	snd_pcm_uframes_t periodsize;
	snd_pcm_uframes_t frames;

	snd_pcm_params pcm_params;
	wav_head head;

	if (argc < 2) {
		printf("usage ./play ");
		return -1;
	}

	fd = open(argv[1], O_RDWR);

	if (fd < 0) {
		printf("file open error");
		return -1;
	}

	parse_wav_file(fd, &head);

	setup_audio_params(&head, &pcm_params);


	//check_wavfile(fd, &hw_params);   //从wav头中分析出的参数，保存在hw_param中

	pcm_open(&pcm_params);
	pcm_set_params(&pcm_params);

	//snd_pcm_hw_params_get_period_time(pcm_params.hw_params, &val, &dir);A
	
	uint32_t buf_frame_size = 0;

	while (1) {
		snd_pcm_hw_params_get_period_size(pcm_params.hw_params, &frames, &dir);
		buf_frame_size = frames * 4; //2byte/smaple, 2 channels
		printf("===> func: %s, line: %d, frames=%d\n", __func__, __LINE__, frames);
		buf_size = buf_frame_size * 12;
		buffer = (char*)malloc(buf_size);
		ret = read(fd, buffer, buf_size);                //3.从wav文件中读取数据
		if (ret == 0) {
			printf("end of file");
			return 0;

		} else if (ret != buf_size) {
			printf("short read");
		}

		for (i = 0; i < buf_size;) {
			ret = snd_pcm_writei(pcm_params.handle, buffer + i, frames);   //4.将读取数据写到driver中进行播放

			if (ret == -EPIPE) {
				//printf("-EPIPE");
				snd_pcm_prepare(pcm_params.handle);
			}
			i += buf_frame_size;
		}
	
		free(buffer);
	}

	pcm_close(&pcm_params);

	return 0;
}
