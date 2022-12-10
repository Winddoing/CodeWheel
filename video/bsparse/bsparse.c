
#include <stdio.h>
#include <libgen.h>
#include <libavcodec/avcodec.h>

#define IN_BUFFER_SIZE 4096

static enum AVCodecID codec_id = AV_CODEC_ID_H264;
static FILE *fp_bs = NULL;
static FILE *fp_size = NULL;

static void usage(char *name)
{
	printf("Usage: %s -in <bitstream_file> [options]\n", basename(name));
	printf("	The size of each frame in the parse stream");
	printf("Options:\n");
	printf("\t-h        Shows this help\n");
	printf("\t-in       Input bitstream\n");
	printf("\t-avc      Specify the input bitstream is AVC (default: AVC)\n");
	printf("\t-hevc     Specify the input bitstream is HEVC (default: AVC)\n");
	printf("\t-size     Size file, store every frame size of Input bitstream\n");
	exit(0);
}

static void parseArgs(int argc, char *argv[])
{
	int i;

	if (argc <= 2) {
		usage(argv[0]);
	}

	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			usage(argv[0]);

		} else if (strcmp(argv[i], "-in") == 0) {
			i++;
			fp_bs = fopen(argv[i], "r");

			if (fp_bs == NULL) {
				printf("can't open input bitstream file: %s\n", argv[i]);
			}

		} else if (strcmp(argv[i], "-avc") == 0) {
			codec_id = AV_CODEC_ID_H264;

		} else if (strcmp(argv[i], "-hevc") == 0) {
			codec_id = AV_CODEC_ID_HEVC;

		} else if (strcmp(argv[i], "-size") == 0) {
			i++;
			fp_size = fopen(argv[i], "w");

			if (fp_size == NULL) {
				printf("can't open output size file: %s\n", argv[i]);
			};
		}
	}
}

static uint32_t decode_frame(AVCodecParserContext *pCodecParserCtx, uint8_t *buf, int size)
{
	printf("width: %d, height: %d\n", pCodecParserCtx->width, pCodecParserCtx->height);
	printf("coded_width: %d, coded_height: %d\n", pCodecParserCtx->coded_width, pCodecParserCtx->coded_height);


	return 0;
}

int main(int argc, char* argv[])
{
	AVCodec *pCodec;
	AVCodecContext *pCodecCtx = NULL;
	AVCodecParserContext *pCodecParserCtx = NULL;
	uint8_t in_buffer[IN_BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE] = {0,};
	uint8_t *cur_ptr;
	int cur_size;
	AVPacket packet;
	parseArgs(argc, argv);
	//av_log_set_level(AV_LOG_DEBUG);
	//avcodec_register_all();
	pCodec = avcodec_find_decoder(codec_id);

	if (!pCodec) {
		printf("Codec not found\n");
		return -1;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);

	if (!pCodecCtx) {
		printf("Could not allocate video codec context\n");
		return -1;
	}

	pCodecParserCtx = av_parser_init(codec_id);

	if (!pCodecParserCtx) {
		printf("Could not allocate video parser context\n");
		return -1;
	}

	//if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
	//    pCodecCtx->flags|= CODEC_FLAG_TRUNCATED;

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec\n");
		return -1;
	}

	if (fp_bs == NULL) {
		printf("No Opened bitstream file\n");
		return -1;
	}

	av_init_packet(&packet);

	while (1) {
		cur_size = fread(in_buffer, 1, IN_BUFFER_SIZE, fp_bs);

		if (cur_size == 0) {
			av_parser_parse2(
					pCodecParserCtx, pCodecCtx,
					&packet.data, &packet.size,
					NULL, 0,
					AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			decode_frame(pCodecParserCtx, packet.data, packet.size);

			//Some Info from AVCodecParserContext
			printf("[Packet]Size:%6d\t", packet.size);

			if (fp_size) {
				fprintf(fp_size, "%d\n", packet.size);
			}

			switch (pCodecParserCtx->pict_type) {
			case AV_PICTURE_TYPE_I:
				printf("Type:I\t");
				break;

			case AV_PICTURE_TYPE_P:
				printf("Type:P\t");
				break;

			case AV_PICTURE_TYPE_B:
				printf("Type:B\t");
				break;

			default:
				printf("Type:Other\t");
				break;
			}

			printf("Number:%4d\n", pCodecParserCtx->output_picture_number);
			printf("End of stream\n");
			break;
		}

		cur_ptr = in_buffer;

		while (cur_size > 0) {
			int len = av_parser_parse2(
							pCodecParserCtx, pCodecCtx,
							&packet.data, &packet.size,
							cur_ptr, cur_size,
							AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
			cur_ptr += len;
			cur_size -= len;

			if (packet.size == 0)
				continue;

			decode_frame(pCodecParserCtx, packet.data, packet.size);

			//Some Info from AVCodecParserContext
			printf("[Packet]Size:%6d\t", packet.size);

			if (fp_size) {
				fprintf(fp_size, "%d\n", packet.size);
			}

			switch (pCodecParserCtx->pict_type) {
			case AV_PICTURE_TYPE_I:
				printf("Type:I\t");
				break;

			case AV_PICTURE_TYPE_P:
				printf("Type:P\t");
				break;

			case AV_PICTURE_TYPE_B:
				printf("Type:B\t");
				break;

			default:
				printf("Type:Other\t");
				break;
			}

			printf("Number:%4d\n", pCodecParserCtx->output_picture_number);
		}
	}

	fclose(fp_bs);

	if (fp_size)
		fclose(fp_size);

	av_parser_close(pCodecParserCtx);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	return 0;
}
