/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : dec.c
 *  Author       : wqshao
 *  Created Time : 2023-04-25 17:31:53
 *  Description  :
 *		gcc dec.c `pkg-config --libs --cflags libavcodec libavutil`
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>

#define INBUF_SIZE 4096

static void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);

		//输出格式yuv420p
		fwrite(frame->data[0], 1, frame->linesize[0] * frame->height, outfile);//Y
		fwrite(frame->data[1], 1, frame->linesize[1] * frame->height/4, outfile);//U
		fwrite(frame->data[2], 1, frame->linesize[2] * frame->height/4, outfile);//V
    }
}

static void dump_bs_info(AVCodecParserContext *parser)
{
	//printf("pts=%ld, dts=%ld, last_pts=%ld, last_dts=%ld\n", parser->pts, parser->dts, parser->last_pts, parser->last_dts);
	if (parser->key_frame == 1) {
		printf("output_picture_number=%d\n", parser->output_picture_number);
		printf("key_frame=%d, pos=%ld, width=%d, height=%d, coded_width=%d, coded_height=%d, format=%d\n",
				parser->key_frame, parser->pos, parser->width, parser->height, parser->coded_width, parser->coded_height, parser->format);
		switch(parser->format) {
		case AV_PIX_FMT_YUV420P:
			printf("AVPixelFormat - AV_PIX_FMT_YUV420P\n");
			break;
		default:
			printf("AVPixelFormat - [%d]\n", parser->format);
			break;
		}
	}

	if (parser->coded_width == 0 || parser->coded_height == 0 || parser->format == -1) {
		printf("bit stream exception. coded_width=%d, coded_height=%d, format=%d\n",
				parser->coded_width, parser->coded_height, parser->format);
	}
}

int main(int argc, char **argv)
{
    const char *filename, *outfilename;
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *c= NULL;
    FILE *f, *outfile;
    AVFrame *frame;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t   data_size;
    int ret;
    AVPacket *pkt;
	const char *codec_type;
	enum AVCodecID codec_id;
	const char *codec_name;

    if (argc <= 3) {
        fprintf(stderr, "Usage: %s <h264|h265> <input file> <output file>\n"
                "And check your input file is encoded by h264 or h265.\n", argv[0]);
        exit(0);
    }

	codec_type  = argv[1];
    filename    = argv[2];
    outfilename = argv[3];

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

	printf("codec_type=%s, filename=%s, outfilename=%s\n", codec_type, filename, outfilename);

	if (!strcmp(codec_type, "h264")) {
		codec_id = AV_CODEC_ID_H264;
		codec_name = "h264";
	} else if (!strcmp(codec_type, "h265")) {
		codec_id = AV_CODEC_ID_H265;
		codec_name = "hevc";
	} else {
        fprintf(stderr, "Usage: %s <h264|h265> <input file> <output file>\n"
                "And check your input file is encoded by h264 or h265.\n", argv[0]);
		exit(0);
	}

    //codec = avcodec_find_decoder(codec_id);
    codec = avcodec_find_decoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
	outfile = fopen(outfilename, "wb");
    if (!outfile) {
        fprintf(stderr, "Could not open %s\n", outfilename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    while (!feof(f)) {
        /* read raw data from the input file */
        data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if (!data_size)
            break;

        /* use the parser to split the data into frames */
        data = inbuf;
        while (data_size > 0) {
            ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;

            if (pkt->size) {
				dump_bs_info(parser);
                decode(c, frame, pkt, outfile);
			}
        }
    }

    /* flush the decoder */
    decode(c, frame, NULL, outfile);

    fclose(f);
	fclose(outfile);

    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}

