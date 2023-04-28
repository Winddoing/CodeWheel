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

#define CONFIG_VASTAPI 1

#include "libavutil/hwcontext.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/opt.h"


#define INBUF_SIZE 4096

static void save_yuv(AVFrame *hw_frame, FILE *outfile)
{
	static int frame_number = 0;

	int ret = 0;

//	AVFrame* frame = av_frame_alloc();
//
//	printf("===> func: %s, line: %d, w=%d, h=%d\n", __func__, __LINE__, hw_frame->width, hw_frame->height);
//
//	frame->format = AV_PIX_FMT_NV12;
//	frame->width  = hw_frame->width;
//	frame->height = hw_frame->height;
//
//	av_frame_get_buffer(frame, 0);

	if ((ret = av_hwframe_transfer_data(frame, hw_frame, 0)) < 0) {
		printf("===> func: %s, line: %d\n", __func__, __LINE__);
		av_frame_free(&frame);
		return;
	}

	if (!frame->data[0] || !frame->data[1] || !frame->data[2]) {
		printf("===> func: %s, line: %d, data0=%p\n", __func__, __LINE__, frame->data[0]);
		printf("===> func: %s, line: %d, data1=%p\n", __func__, __LINE__, frame->data[1]);
		printf("===> func: %s, line: %d, data2=%p\n", __func__, __LINE__, frame->data[2]);
	}

	printf("===> func: %s, line: %d, linesize0=%d\n", __func__, __LINE__, frame->linesize[0]);
	printf("===> func: %s, line: %d, linesize1=%d\n", __func__, __LINE__, frame->linesize[1]);
	printf("===> func: %s, line: %d, linesize2=%d\n", __func__, __LINE__, frame->linesize[2]);

	printf("===> func: %s, line: %d, w=%d, h=%d\n", __func__, __LINE__, frame->width, frame->height);
	printf("===> func: %s, line: %d, format=%d\n", __func__, __LINE__, frame->format);

	fwrite(frame->data[0], 1, frame->width*frame->height, outfile);//Y
	fwrite(frame->data[1], 1, frame->width*frame->height/2, outfile);//UV

	av_frame_free(&frame);
}

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

		printf("===> func: %s, line: %d, w=%d, h=%d\n", __func__, __LINE__, frame->width, frame->height);
		printf("===> func: %s, line: %d, format=%d, AV_PIX_FMT_NV12=%d, AV_PIX_FMT_VASTAPI=%d\n", __func__, __LINE__, frame->format, AV_PIX_FMT_NV12, AV_PIX_FMT_VASTAPI);

		save_yuv(frame, outfile);

		//fwrite(frame->data[0], 1, frame->width*frame->height, outfile);//Y
		//fwrite(frame->data[1], 1, frame->width*frame->height/2, outfile);//UV
    }
}

static int set_hwframe_ctx(AVCodecContext *ctx, AVBufferRef *hw_device_ctx,int flag)
{
	AVBufferRef *hw_frames_ref;
	AVHWFramesContext *frames_ctx = NULL;
	int err = 0;

	if (!(hw_frames_ref = av_hwframe_ctx_alloc(hw_device_ctx))) {
		fprintf(stderr, "Failed to create VASTAPI frame context.\n");
		return -1;
	}

	frames_ctx = (AVHWFramesContext *)(hw_frames_ref->data);

#if 0
	if (strcmp(input_params->pixel_format, "yuv420p") == 0) {
		frames_ctx->sw_format = AV_PIX_FMT_YUV420P;

	} else if (strcmp(input_params->pixel_format, "nv12") == 0)
		frames_ctx->sw_format = AV_PIX_FMT_NV12;

	else if (strcmp(input_params->pixel_format, "nv21") == 0)
		frames_ctx->sw_format = AV_PIX_FMT_NV21;

	else if (strcmp(input_params->pixel_format, "rgb0") == 0)
		frames_ctx->sw_format = AV_PIX_FMT_RGB0;

	else
		frames_ctx->sw_format = AV_PIX_FMT_NV12;
#endif

	printf("===> func: %s, line: %d, w=%d, h=%d\n", __func__, __LINE__, ctx->width, ctx->height);
	frames_ctx->sw_format = AV_PIX_FMT_NV12;

	frames_ctx->format    = AV_PIX_FMT_VASTAPI;
	//frames_ctx->width     = ctx->width;
	//frames_ctx->height    = ctx->height;
	frames_ctx->width     = 1920;
	frames_ctx->height    = 1080;
	frames_ctx->initial_pool_size = 32;
	frames_ctx->frame_buffer_flag = flag;

	if ((err = av_hwframe_ctx_init(hw_frames_ref)) < 0) {
		fprintf(stderr, "Failed to initialize VASTAPI frame context."
				"Error code: %s\n", av_err2str(err));
		av_buffer_unref(&hw_frames_ref);
		return err;
	}

	ctx->hw_frames_ctx = av_buffer_ref(hw_frames_ref);

	if (!ctx->hw_frames_ctx)
		err = AVERROR(ENOMEM);

	av_buffer_unref(&hw_frames_ref);
	return err;
}
static enum AVPixelFormat get_vastapi_format(AVCodecContext *ctx,
		const enum AVPixelFormat *pix_fmts)  //获取formart
{
	const enum AVPixelFormat *p;

	for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++) {
		if (*p == AV_PIX_FMT_VASTAPI)
			return *p;
	}

	fprintf(stderr, "Unable to decode this file using VA-API.\n");
	return AV_PIX_FMT_NONE;
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
	printf("===> func: %s, line: %d, AV_INPUT_BUFFER_PADDING_SIZE=%d\n", __func__, __LINE__, AV_INPUT_BUFFER_PADDING_SIZE);

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

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	static AVBufferRef *hw_device_ctx = NULL;
	ret = av_hwdevice_ctx_create(&hw_device_ctx, AV_HWDEVICE_TYPE_VASTAPI, "/dev/dri/renderD130", NULL, 0);
	if (ret < 0) {
		fprintf(stderr, "Failed to create a VASTAPI device. Error code: %s\n", av_err2str(ret));
		return -1;
	}

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	c->hw_device_ctx = av_buffer_ref(hw_device_ctx);

	if (!c->hw_device_ctx) {
		fprintf(stderr, "A hardware device reference create failed.\n");
		ret = AVERROR(ENOMEM);
		return -1;
	}

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	//set_hwframe_ctx(c, hw_device_ctx, 0);
	//c->get_format    = get_vastapi_format;
	//c->framerate = av_guess_frame_rate(ifmt_ctx[chn], video, NULL);
	//c->framerate = (AVRational){30, 1};;

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
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

            if (pkt->size)
                decode(c, frame, pkt, outfile);
        }
    }

    /* flush the decoder */
    decode(c, frame, NULL, outfile);

    fclose(f);
	fclose(outfile);

	av_buffer_unref(&hw_device_ctx); //释放硬件资源
    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}

