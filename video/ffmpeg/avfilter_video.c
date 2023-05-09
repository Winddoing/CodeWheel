/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : avfilter_video.c
 *  Author       : wqshao
 *  Created Time : 2023-03-20 16:40:26
 *  Description  :
 *			ffmpeg version 4.2.7
 *			sudo apt install libavfilter-dev
 *			gcc avfilter_video.c `pkg-config --libs --cflags libavfilter libavutil`
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>


int main(int argc, char* argv[])
{
	if (argc < 4) {
		fprintf(stdout, "usage: %s <input yuv420p> <width> <height>\n", argv[0]);
		return -1;
	}

    const char *in_filename = argv[1];
    int in_width = atoi(argv[2]);
    int in_height = atoi(argv[3]);
    const char *out_filename = "output.yuv420p";
	//const char *filter_descr = "lutyuv='u=128:v=128'";
	//const char *filter_descr = "boxblur";
	//const char *filter_descr = "hflip";
	//const char *filter_descr = "hue='h=60:s=-3'";
	//const char *filter_descr = "crop=2/3*in_w:2/3*in_h";
	//const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
	const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=green:fontsize=30:text='Hello World':x=0:y=100";
    if (in_width <= 0 || in_height <= 0) {
        fprintf(stderr, "invalid parameters.\n");
		return -1;
    } 

    int ret;
    AVFrame *frame_in;
    AVFrame *frame_out;
    unsigned char *frame_buffer_in;
    unsigned char *frame_buffer_out;

    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;
    AVFilterGraph *filter_graph;
    static int video_stream_index = -1;

    FILE *in_fp = fopen(in_filename, "rb+");
    if (!in_fp) {
        printf("Error open input file.\n");
        return -1;
    }

    FILE *out_fp = fopen(out_filename, "wb+");
    if(!out_fp){
        printf("Error open output file.\n");
        return -1;
    }

    char args[512];
    const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    if (!buffersrc) {
        fprintf(stderr, "Can't find source fiter 'buffer'\n");
        return -1; 
    }
    const AVFilter *buffersink = avfilter_get_by_name("buffersink");
    if (!buffersink) {
        fprintf(stderr, "Can't find sink fiter 'buffersink'\n");
        return -1; 
    }
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
    enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };
    AVBufferSinkParams *buffersink_params;

	filter_graph = avfilter_graph_alloc();
	if (!outputs || !inputs || !filter_graph) {
		printf("alloc error.\n");
		return -1;
	}

    /* buffer video source: the decoded frames from the decoder will be inserted here. */
    snprintf(args, sizeof(args),
        "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
        in_width,in_height,AV_PIX_FMT_YUV420P,
        1, 25,1,1);

    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
        args, NULL, filter_graph);
    if (ret < 0) {
        printf("Cannot create buffer source\n");
        return ret;
    }

    /* buffer video sink: to terminate the filter chain. */
    buffersink_params = av_buffersink_params_alloc();
    buffersink_params->pixel_fmts = pix_fmts;
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
        NULL, buffersink_params, filter_graph);
    av_free(buffersink_params);
    if (ret < 0) {
        fprintf(stderr, "Cannot create buffer sink, error:%s\n", av_err2str(ret));
        return ret;
    }

    /*
     * Set the endpoints for the filter graph. The filter_graph will
     * be linked to the graph described by filters_descr.
     */

    /*
     * The buffer source output must be connected to the input pad of
     * the first filter described by filters_descr; since the first
     * filter input label is not specified, it is set to "in" by
     * default.
     */
    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;

    /*
     * The buffer sink input must be connected to the output pad of
     * the last filter described by filters_descr; since the last
     * filter output label is not specified, it is set to "out" by
     * default.
     */
    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
        &inputs, &outputs, NULL)) < 0) {
        fprintf(stderr, "avfilter_graph_parse_ptr failed, error:%s\n", av_err2str(ret));
        return ret;
    }

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0) {
        fprintf(stderr, "avfilter_graph_config failed, error:%s\n", av_err2str(ret));
        return ret;
    }

    frame_in = av_frame_alloc();
    frame_buffer_in = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width,in_height,1));
    av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
        AV_PIX_FMT_YUV420P, in_width, in_height, 1);

    frame_out = av_frame_alloc();
    frame_buffer_out = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width,in_height,1));
    av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,
        AV_PIX_FMT_YUV420P, in_width, in_height, 1);

    frame_in->width = in_width;
    frame_in->height = in_height;
    frame_in->format = AV_PIX_FMT_YUV420P;
    
    while (1) {
        if(fread(frame_buffer_in, 1, in_width*in_height*3/2, in_fp) != in_width * in_height * 3/2){
            break;
        }
        //input Y,U,V
        frame_in->data[0] = frame_buffer_in;
        frame_in->data[1] = frame_buffer_in + in_width * in_height;
        frame_in->data[2] = frame_buffer_in + in_width * in_height * 5/4;

        if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0) {
            printf( "Error while add frame.\n");
            break;
        }

        /* pull filtered pictures from the filtergraph */
        ret = av_buffersink_get_frame(buffersink_ctx, frame_out);
        if (ret < 0)
            break;

        //output Y,U,V
        if(frame_out->format == AV_PIX_FMT_YUV420P){
            for(int i = 0; i < frame_out->height; i++){
                fwrite(frame_out->data[0] + frame_out->linesize[0] * i, 1, frame_out->width, out_fp);
            }
            for(int i = 0; i < frame_out->height/2; i++){
                fwrite(frame_out->data[1] + frame_out->linesize[1] * i, 1, frame_out->width / 2, out_fp);
            }
            for(int i = 0; i < frame_out->height/2; i++){
                fwrite(frame_out->data[2] + frame_out->linesize[2] * i, 1, frame_out->width / 2, out_fp);
            }
        }
        fprintf(stdout, "Process 1 frame!\n");
        av_frame_unref(frame_out);
    }

    fclose(in_fp);
    fclose(out_fp);
    av_frame_free(&frame_in);
    av_frame_free(&frame_out);
    avfilter_graph_free(&filter_graph);
    return 0;
}
