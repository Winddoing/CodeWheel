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
#include <libgen.h> //basename

//refer to alsa-utils-1.2.4
#define COMPOSE_ID(a,b,c,d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define WAV_RIFF        COMPOSE_ID('R','I','F','F')
#define WAV_RIFX        COMPOSE_ID('R','I','F','X')
#define WAV_WAVE        COMPOSE_ID('W','A','V','E')
#define WAV_FMT         COMPOSE_ID('f','m','t',' ')
#define WAV_DATA        COMPOSE_ID('d','a','t','a')

// RIFF 头部
typedef struct {
	union {
		uint32_t	chunkId;            // 块标识符，通常为 'RIFF'（0x46464952），用于标识这是一个 RIFF 文件
		char		chunkIdFlag[4];     // 块标识符的字符形式，便于调试和打印
	} magic;
	uint32_t		chunkSize;          // 文件总长度减去 chunkId 和 chunkSize（即文件大小 - 8 字节）
	union {
		uint32_t	type;               // 文件类型标识，通常为 'WAVE'（0x45564157），表示是 WAV 格式
		char		typeFlag[4];        // 文件类型标识的字符形式，便于调试和打印
	} format;
} RiffHeader;

// fmt 子块（描述音频格式信息）
typedef struct {
	union {
		uint32_t	subChunk1Id;        // 子块标识符，通常为 'fmt '（0x20746d66），最后一位为空格
		char		subChunk1IdFlag[4]; // 子块标识符的字符形式，便于调试和打印
	} magic;
	uint32_t		subChunk1Size;      // 子块数据长度，PCM 格式通常为 16
	uint16_t		audioFormat;        // 音频格式代码，1 表示 PCM，其他值表示压缩格式
	uint16_t		channelsNum;        // 声道数，1 表示单声道，2 表示立体声
	uint32_t		sampleRate;         // 采样率（每秒采样数），如 44100 Hz
	uint32_t		byteRate;           // 每秒字节数 = sampleRate * channelsNum * sampleBits / 8
	uint16_t		blockAlign;         // 每个采样点的字节数 = channelsNum * sampleBits / 8
	uint16_t		sampleBits;         // 每个样本的位数（如 16 位）
} FmtSubChunk;

// data 子块（包含实际音频数据）
typedef struct {
	union {
		uint32_t	subChunk2Id;        // 子块标识符，通常为 'data'（0x61746164）
		char		subChunk2IdFlag[4]; // 子块标识符的字符形式，便于调试和打印
	} magic;
	uint32_t		subChunk2Size;      // 音频数据的字节数
} DataSubChunk;

// WAV 文件头结构（包含 RIFF 头、fmt 子块和 data 子块）
typedef struct {
	RiffHeader		riff;               // RIFF 文件头
	FmtSubChunk		fmt;                // 格式描述子块
	DataSubChunk		data;               // 数据子块
} wav_head;

struct audio_convert {
	wav_head head;
	int ifd, ofd;
	int raw_size;
};

enum func {
	ACT_FUNC_WAV_INFO,
	ACT_FUNC_DUMP_PCM_DATA,
	ACT_FUNC_24_LE_3LE,
	ACT_FUNC_24_3LE_LE,
	ACT_FUNC_32_TO_16_LE,
	ACT_FUNC_GEN_WAV,
};

static char* func_name[] = {"info", "dump", "24_le_3le", "24_3le_le", "32_to_16_le", "gen"};

struct cli_cfg {
	char in_file[256];
	char out_file[256];
	enum func act_func;
};

static void dump_wav_info(wav_head *head)
{
	printf("### WAV Head info:\n");
	printf("##   RIFF subblock:\n");
	printf("          chunkId : 0x%08x(%s)\n", head->riff.magic.chunkId, head->riff.magic.chunkIdFlag);
	printf("        chunkSize : %u\n", head->riff.chunkSize);
	printf("             type : 0x%08x(%s)\n", head->riff.format.type, head->riff.format.typeFlag);
	printf("## Format subblock:\n");
	printf("      subChunk1Id : 0x%08x(%s)\n", head->fmt.magic.subChunk1Id, head->fmt.magic.subChunk1IdFlag);
	printf("    subChunk1Size : %u\n", head->fmt.subChunk1Size);
	printf("      audioFormat : %u\n", head->fmt.audioFormat);
	printf("      channelsNum : %u\n", head->fmt.channelsNum);
	printf("       sampleRate : %u\n", head->fmt.sampleRate);
	printf("         byteRate : %u\n", head->fmt.byteRate);
	printf("       blockAlign : %u\n", head->fmt.blockAlign);
	printf("       sampleBits : %u\n", head->fmt.sampleBits);
	printf("##   Data subblock:\n");
	printf("      subChunk2Id : 0x%08x(%s)\n", head->data.magic.subChunk2Id, head->data.magic.subChunk2IdFlag);
	printf("    subChunk2Size : %u\n", head->data.subChunk2Size);
	printf("Wav Head size: %lu Byte\n\n", (unsigned long)sizeof(wav_head));
}

static int dump_pcm_data(struct audio_convert *act, uint16_t data_len_bit)
{
	int i = 0, j = 0;
	uint8_t buf[4];
	uint32_t data_len_byte = data_len_bit / 8;
	//uint32_t data_size = act->raw_size > 4096 ? 4096 : act->raw_size;
	uint32_t data_size = act->raw_size;

	printf("%s: data_size=%d, data_len_bit=%d, data_len_byte=%d\n", __func__, data_size, data_len_bit, data_len_byte);
	for (i = 0; i < data_size; i = i + data_len_byte) {
		if (-1 == read(act->ifd, buf, data_len_byte)) {
			printf("%s Read (%s)\n", __func__, strerror(errno));
			return -1;
		}
#if 0
		for (j = 0; j < 4; j++) {
			printf("%02x", buf[j]);
		}
#else
		//小端输出
		for (j = data_len_byte - 1; j >= 0; j--) {
			printf("%02x", buf[j]);
		}
#endif

#if 0
		// 每4列输出一行
		printf(" ");
		if (!((i + data_len_byte) % (4 * data_len_byte)))
			printf("\n");
#else
		// 单列输出
		if (!((i + data_len_byte) % (1 * data_len_byte)))
			printf("\n");

#endif
	}
	printf("\n");

	return 0;
}

static int s24_le_to_s24_3le(struct audio_convert *act)
{
	int i = 0;
	uint8_t buf[4];
	int of_raw_sz = 0;
	int of_duration_time = 0;
	wav_head ohead;

	printf("%s: start\n", __func__);

	dump_wav_info(&act->head);

	if (act->head.fmt.sampleBits != 24 && act->head.fmt.blockAlign != 8) {
		printf("E(%s): input audio file format does not match.\n", __func__);
		return -1;
	}

	of_raw_sz = act->raw_size * 3 / 4;
	of_duration_time = (of_raw_sz * 8) /
		(act->head.fmt.sampleBits * act->head.fmt.channelsNum * act->head.fmt.sampleRate);

	memcpy(&ohead, &act->head, sizeof(ohead));

	ohead.fmt.blockAlign = (act->head.fmt.sampleBits * act->head.fmt.channelsNum / 8);  //原头部为8,不调整无法播放
	ohead.fmt.byteRate = of_raw_sz / of_duration_time;
	ohead.data.subChunk2Size = of_raw_sz;

	dump_wav_info(&ohead);

	if ( -1 == write(act->ofd, &ohead, sizeof(ohead))) {
		printf("%s Write Head (%s)\n", __func__, strerror(errno));
		return -1;
	}

	/*
	 *           MSB                           LSB
	 *           4th byte  3rd byte  2nd byte  1st byte   alignment
	 * S24_LE:   00000000  xxxxxxxx  xxxxxxxx  xxxxxxxx   32 bits
	 * S24_3LE:	       xxxxxxxx  xxxxxxxx  xxxxxxxx   24 bits
	 */
	for (i = 0; i < act->raw_size; i += 4) {
		/* 1. read 4 bytes S24_LE [xx xx xx 00] */
		if (-1 == read(act->ifd, buf, 4)) {
			printf("%s Read (%s)\n", __func__, strerror(errno));
			return -1;
		}

		if (0) {
			int j=0;
			for (j = 4 - 1; j <= 0; j--) {
				printf("%02x ", buf[j]);
			}
			printf("\n");
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

static int s24_3le_to_s24_le(struct audio_convert *act)
{
	int i = 0;
	uint8_t buf[4];
	int of_raw_sz = 0;
	int of_duration_time = 0;
	uint32_t of_data_bits = 32; //for s24_le
	wav_head ohead;

	printf("%s: start\n", __func__);

	dump_wav_info(&act->head);

	if (act->head.fmt.sampleBits != 24 && act->head.fmt.blockAlign != 6) {
		printf("E(%s): input audio file format does not match.\n", __func__);
		return -1;
	}

	of_raw_sz = act->raw_size * 4 / 3;
	of_duration_time = (of_raw_sz * 8) /
		(of_data_bits * act->head.fmt.channelsNum * act->head.fmt.sampleRate);

	memcpy(&ohead, &act->head, sizeof(ohead));

	ohead.fmt.blockAlign = (of_data_bits * act->head.fmt.channelsNum / 8);  //转换为s24_le格式时，blockSize=8,不能为6,否则无法播放
	ohead.fmt.byteRate = of_raw_sz / of_duration_time;
	ohead.data.subChunk2Size = of_raw_sz;

	dump_wav_info(&ohead);

	if ( -1 == write(act->ofd, &ohead, sizeof(ohead))) {
		printf("%s Write Head (%s)\n", __func__, strerror(errno));
		return -1;
	}

	/*
	 *           MSB                           LSB
	 *           4th byte  3rd byte  2nd byte  1st byte   alignment
	 * S24_3LE:            xxxxxxxx  xxxxxxxx  xxxxxxxx   24 bits
	 * S24_LE:   00000000  xxxxxxxx  xxxxxxxx  xxxxxxxx   32 bits
	 */
	for (i = 0; i < act->raw_size; i += 3) {
		memset(buf, 0, sizeof(buf));
		/* 1. read 3 bytes S24_LE [xx xx xx] */
		if (-1 == read(act->ifd, buf, 3)) {
			printf("%s Read (%s)\n", __func__, strerror(errno));
			return -1;
		}

		if (0) {
			int j=0;
			for (j = 3 - 1; j <= 0; j--) {
				printf("%02x ", buf[j]);
			}
			printf("\n");
		}

		/* 2. write 4 bytes [00 xx xx xx] */
		if ( -1 == write(act->ofd, buf, 4)) {
			printf("%s Write (%s)\n", __func__, strerror(errno));
			return -1;
		}
	}
	printf("%s: end\n", __func__);

	return 0;
}

static int s32_le_to_s16_le(struct audio_convert *act)
{
	int i = 0;
	uint8_t buf[4];
	int of_raw_sz = 0;
	int of_duration_time = 0;
	uint32_t of_data_bits = 16; //for s16_le
	wav_head ohead;

	printf("%s: start\n", __func__);

	dump_wav_info(&act->head);

	if (act->head.fmt.sampleBits != 32 && act->head.fmt.blockAlign != 8) {
		printf("E(%s): input audio file format does not match.\n", __func__);
		return -1;
	}

	of_raw_sz = act->raw_size / 2;
	of_duration_time = (of_raw_sz * 8) /
		(of_data_bits * act->head.fmt.channelsNum * act->head.fmt.sampleRate);

	memcpy(&ohead, &act->head, sizeof(ohead));

	ohead.fmt.sampleBits = of_data_bits;
	ohead.fmt.blockAlign = (of_data_bits * act->head.fmt.channelsNum / 8);
	ohead.fmt.byteRate = of_raw_sz / of_duration_time;
	ohead.data.subChunk2Size = of_raw_sz;

	dump_wav_info(&ohead);

	if ( -1 == write(act->ofd, &ohead, sizeof(ohead))) {
		printf("%s Write Head (%s)\n", __func__, strerror(errno));
		return -1;
	}

	/*
	 * 直接截取，将低16bit丢弃
	 *           MSB                           LSB
	 *           4th byte  3rd byte  2nd byte  1st byte   alignment
	 * S32_LE:   xxxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx   32 bits
	 * S16_LE:   xxxxxxxx  xxxxxxxx  		      16 bits
	 */
	for (i = 0; i < act->raw_size; i += act->head.fmt.sampleBits / 8) {
		memset(buf, 0, sizeof(buf));
		/* 1. read 4 bytes S32_LE [xx xx xx] */
		if (-1 == read(act->ifd, buf, act->head.fmt.sampleBits / 8)) {
			printf("%s Read (%s)\n", __func__, strerror(errno));
			return -1;
		}

		if (0) {
			int j=0;
			for (j = 3 - 1; j <= 0; j--) {
				printf("%02x ", buf[j]);
			}
			printf("\n");
		}

		/* 2. write high 2 bytes [xx xx] */
		if ( -1 == write(act->ofd, buf + 2, ohead.fmt.sampleBits / 8)) {
			printf("%s Write (%s)\n", __func__, strerror(errno));
			return -1;
		}
	}
	printf("%s: end\n", __func__);

	return 0;
}

static int gen_wav_data(int rate, int bits, int chan)
{
	int wav_data_time = 60;		//生成wav数据时间60s
	int wav_data_size = wav_data_time * rate * bits * chan / 8;
	char wav_file_name[64] = {0};
	int wav_file_size = 0;
	int fd = -1, ret = 0;
	wav_head head;

	printf("%s: start\n", __func__);

	memset(&head, 0, sizeof(head));
	head.riff.magic.chunkId     = WAV_RIFF;
	//head.riff.chunkSize       = xx:
	head.riff.format.type       = WAV_WAVE;
	head.fmt.magic.subChunk1Id  = WAV_FMT;
	head.fmt.subChunk1Size      = 16;
	head.fmt.audioFormat        = 1;
	head.fmt.channelsNum        = chan;
	head.fmt.sampleRate         = rate;
	head.fmt.byteRate           = (rate * bits * chan) / 8;
	head.fmt.blockAlign         = (bits == 24) ? 32 * chan / 8 : bits * chan / 8;
	head.fmt.sampleBits         = bits;
	head.data.magic.subChunk2Id = WAV_DATA;
	head.data.subChunk2Size     = wav_data_size;

	sprintf(wav_file_name, "audio_%d_%dbits_%dch.wav", rate, bits, chan);
	fd = open(wav_file_name, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	if (-1 == fd) {
		printf("Cannot open file [%s] (%s)\n", wav_file_name, strerror(errno));
		return -1;
	}

	// 创建单个循环块 (1024字节)
	int i = 0, j = 0;
	int wav_block_size = 256 * 4 + 4;
	unsigned char block[wav_block_size];
	for (i = 0; i < 256; i++) {
		// 每个模式填充4个相同的字节
		for (j = 0; j < 4; j++) {
			block[i * 4 + j] = (unsigned char)i;
		}
	}
	for (j = 0; j < 4; j++) {
		block[i * 4 + j] = 0x5a;
	}

	if (write(fd, &head, sizeof(head)) != sizeof(head)) {
		printf("%s Write Head (%s)\n", __func__, strerror(errno));
		return -1;
	}
	wav_file_size += sizeof(head);

	while (1) {
		if (write(fd, block, wav_block_size) != wav_block_size) {
			printf("%s fwrite failed. (%s)\n", __func__, strerror(errno));
			close(fd);
			return -1;
		}
		wav_data_size -= wav_block_size;
		if (wav_data_size == 0) {
			printf("%s Write wav data finish.\n", __func__);
			break;
		}

		if (wav_data_size < wav_block_size)
			wav_block_size = wav_data_size;

		wav_file_size += wav_block_size;
	}

	head.riff.chunkSize = wav_file_size - 4;

	lseek(fd, 0, SEEK_SET);
	ret = write(fd, &head, sizeof(head));
	if (ret != sizeof(head)) {
		printf("%s write wav head error. ret=%d\n", __func__, ret);
		return -1;
	}

	printf("%s file name: [%s]\n", __func__, wav_file_name);

	// 关闭文件
	close(fd);

	printf("%s: end\n", __func__);
	return 0;
}

static int audio_test_functions(struct audio_convert *act, enum func act_func)
{
	printf("%s: func=%d(%s)\n", __func__, act_func, func_name[act_func]);

	switch (act_func) {
	case ACT_FUNC_WAV_INFO:
		dump_wav_info(&act->head);
		break;
	case ACT_FUNC_DUMP_PCM_DATA:
		dump_wav_info(&act->head);
		dump_pcm_data(act, !(act->head.fmt.blockAlign % 8) ? (act->head.fmt.sampleBits >= 24 ? 32 : act->head.fmt.sampleBits) : act->head.fmt.sampleBits);
		break;
	case ACT_FUNC_24_LE_3LE:
		s24_le_to_s24_3le(act);
		break;
	case ACT_FUNC_24_3LE_LE:
		s24_3le_to_s24_le(act);
		break;
	case ACT_FUNC_32_TO_16_LE:
		s32_le_to_s16_le(act);
		break;
	default:
		dump_pcm_data(act, 32);
		break;
	}

	return 0;
}

static void usage(char *name)
{
	printf("Usage: %s -<func> -in <in_file> -out <out_file>\n", basename(name));
	printf("Function:\n");
	printf("\t-%s		show wav header info\n", func_name[0]);
	printf("\t-%s		dump pcm data\n", func_name[1]);
	printf("\t-%s	S24_LE convert S24_3LE\n", func_name[2]);
	printf("\t-%s	S24_3LE convert S24_LE\n", func_name[3]);
	printf("\t-%s	S32_LE convert S16_LE\n", func_name[4]);
	printf("\t-%s		Generate wav data 32-bit, mono\n", func_name[5]);
	printf("Options:\n");
	printf("\t-in <wav>	input wav file\n");
	printf("\t-out <wav>	output wav file\n");
	printf("\t-h		Shows this help\n");
}

static int parse_cli_args(int argc, char *argv[], struct cli_cfg *cfg)
{
	int i = 0, min_argc_cnt = 0;
	char func_str[32] = {0};

	for (i = 0; i < argc; i++) {
		if (strcmp("-h", argv[i]) == 0) {
			usage(argv[0]);
		} else if (strcmp("-in", argv[i]) == 0) {
			i++;
			sprintf(cfg->in_file, "%s", argv[i]);
		} else if (strcmp("-out", argv[i]) == 0) {
			i++;
			sprintf(cfg->out_file, "%s", argv[i]);
		} else {
			for (int j = 0; j < sizeof(func_name) / sizeof(func_name[0]); j++) {
				sprintf(func_str, "-%s", func_name[j]);
				//printf("-->j=%d, func_str=%s, argv[%d]=%s\n", j, func_str, i, argv[i]);
				if (strcmp(func_str, argv[i]) == 0) {
					cfg->act_func = j;
				}
			}
		}
	}

	if (cfg->act_func == ACT_FUNC_GEN_WAV)
		min_argc_cnt = 1;
	else
		min_argc_cnt = 3;

	//printf("-----i=%d, argc=%d, min_argc_cnt=%d\n", i, argc, min_argc_cnt);
	if (argc < min_argc_cnt) {
		usage(argv[0]);
		return -1;
	}

	printf("-------------------------------------------------------------\n");
	printf("Dump cmdline args:\n");
	printf("     func : %d(%s)\n", cfg->act_func, func_name[cfg->act_func]);
	printf("  in_file : [%s]\n", cfg->in_file);
	printf(" out_file : [%s]\n", cfg->out_file);
	printf("-------------------------------------------------------------\n");

	return 0;
}

int main(int argc, char *argv[])
{
	struct audio_convert act = {0};
	struct cli_cfg cfg = {0};
	struct stat fstat;
	int fsize = 0;

	if (parse_cli_args(argc, argv, &cfg)) {
		return -1;
	}

	if (cfg.act_func == ACT_FUNC_GEN_WAV) {
		return gen_wav_data(48000, 32, 1);
	}

	printf("input file: [%s]\n", cfg.in_file);

	act.ifd = act.ofd = -1;

	act.ifd = open(cfg.in_file, O_RDONLY);
	if (-1 == act.ifd) {
		printf("Cannot open input file [%s] (%s)\n", cfg.in_file, strerror(errno));
		exit(1);
	}

	if (stat(cfg.in_file, &fstat) == -1) {
		printf("Cannot get fstat [%s] (%s)\n", cfg.in_file, strerror(errno));
		exit(1);
	}
	fsize = fstat.st_size;

	if(-1 == read(act.ifd, &act.head, sizeof(act.head))) {
		printf("Cannot read [%s] (%s)\n", cfg.in_file, strerror(errno));
		exit(1);
	}

	if ((cfg.act_func != ACT_FUNC_WAV_INFO) && (cfg.act_func != ACT_FUNC_DUMP_PCM_DATA)) {
		printf("output file: [%s]\n", cfg.out_file);
		act.ofd = open(cfg.out_file, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
		if (-1 == act.ofd) {
			printf("Cannot open output file [%s] (%s)\n", cfg.out_file, strerror(errno));
			exit(1);
		}
	}

	if(act.head.riff.format.type == WAV_WAVE) {
		printf("### File Type WAV\n");
		//dump_wav_info(&act.head);

		act.raw_size = fsize - sizeof(act.head);
		audio_test_functions(&act, cfg.act_func);

	} else {
		printf("### File Type RAW\n");

		act.raw_size = fsize;
		audio_test_functions(&act, cfg.act_func);
	}

	if (act.ifd != -1)
		close(act.ifd);
	if (act.ofd != -1)
		close(act.ofd);

	return 0;
}
